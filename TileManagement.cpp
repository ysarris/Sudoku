#include "Game.h"
#include "Tile_info.h"		// for TILE_SIZE_PIXELS
#include "Walls_info.h"		// for WALL_THICKNESS_TILES
#include "Window_info.h"	// for WINDOW_HEIGHT_TILES and WINDOW_WIDTH_TILES

static constexpr auto VERTICES_IN_QUAD = 4;

static bool IsGate(int tileValue)
{
	return (tileValue >= CLOSED_GATE_TILE_VALUE) && (tileValue <= OPEN_GATE_TILE_VALUE);
}

static int CalcPlayerRowOnBoard(float playerPosY)
{
	auto row = static_cast<int>(playerPosY / TILE_SIZE_PIXELS);

	// Within wall bounds ?
	if (row < WALL_THICKNESS_TILES) { row = WALL_THICKNESS_TILES; }
	else if (row > BOARD_SIZE) { row = BOARD_SIZE; }

	// Subtract wall thickness to translate global tile layout row to Sudoku board local row
	return row - WALL_THICKNESS_TILES;
}

static int CalcPlayerColumnOnBoard(float playerPosX)
{
	auto column = static_cast<int>(playerPosX / TILE_SIZE_PIXELS);

	// Within wall bounds ?
	if (column < WALL_THICKNESS_TILES) { column = WALL_THICKNESS_TILES; }
	else if (column > BOARD_SIZE) { column = BOARD_SIZE; }

	// Subtract wall thickness to translate global tile layout columnn to Sudoku board local column
	return column - WALL_THICKNESS_TILES;
}

// Returns which wall a given tile is part of
// Corner cases are treated as part of top and bottom walls
// Non-wall tiles will return Direction::NONE
static Direction WhichWall(int column, int row)
{
	auto whichWall = Direction::NONE;

	// Top wall
	if (row < WALL_THICKNESS_TILES)
		whichWall = Direction::UP;
	// Right wall
	else if (column >= BOARD_SIZE + WALL_THICKNESS_TILES && row >= WALL_THICKNESS_TILES && row < BOARD_SIZE + WALL_THICKNESS_TILES)
		whichWall = Direction::RIGHT;
	// Bottom wall
	else if (row >= BOARD_SIZE + WALL_THICKNESS_TILES)
		whichWall = Direction::DOWN;
	// Left wall
	else if (column < WALL_THICKNESS_TILES && row >= WALL_THICKNESS_TILES && row < BOARD_SIZE + WALL_THICKNESS_TILES)
		whichWall = Direction::LEFT;

	return whichWall;
}

void Game::CreateGates()
{
	for (unsigned int column = 0; column < WINDOW_WIDTH_TILES; column++)
	{
		for (unsigned int row = 0; row < WINDOW_HEIGHT_TILES; row++)
		{
			// Only wall tiles can be a gate
			auto wall = WhichWall(column, row);

			// Wall tile with gate ?
			if (wall != Direction::NONE && IsGate(m_TileLayout[row][column]))
			{
				// Calculate gate position
				auto pos = sf::Vector2f(column + 0.5f, row + 0.5f) * TILE_SIZE_PIXELS;
				// Gate exit faces away from the wall
				auto facing = GetOppositeDirection(wall);

				m_Gates.emplace_back(std::make_unique<Gate>(pos, facing, m_TileLayout[row][column]));
			}
		}
	}
}

// Sets the tile vertex array using the tile layout array
void Game::SetTVAfromTLA()
{
	// Use quads, need four vertices for each tile
	m_Tiles.setPrimitiveType(sf::Quads);
	m_Tiles.resize(WINDOW_WIDTH_TILES * WINDOW_HEIGHT_TILES * VERTICES_IN_QUAD);

	// Start from first vertex in array
	int curVertex = 0;

	for (unsigned int column = 0; column < WINDOW_WIDTH_TILES; column++)
	{
		for (unsigned int row = 0; row < WINDOW_HEIGHT_TILES; row++)
		{
			auto posX = static_cast<float>(column);
			auto posY = static_cast<float>(row);
			// Position each vertex in current quad
			//	0 -- 1
			//	|    |
			//	3 -- 2
			m_Tiles[curVertex + 0].position = sf::Vector2f(posX, posY) * TILE_SIZE_PIXELS;
			m_Tiles[curVertex + 1].position = sf::Vector2f(posX + 1, posY) * TILE_SIZE_PIXELS;
			m_Tiles[curVertex + 2].position = sf::Vector2f(posX + 1, posY + 1) * TILE_SIZE_PIXELS;
			m_Tiles[curVertex + 3].position = sf::Vector2f(posX, posY + 1) * TILE_SIZE_PIXELS;

			// Calculate offset using sprite sheet tile value
			auto verticalOffset = TILE_SIZE_PIXELS * m_TileLayout[row][column];

			// Determine tile rotation
			// Top wall and non-wall tiles dont need to be rotated
			auto wall = WhichWall(column, row);
			int rotationOffset = 0;
			// Right wall
			if (wall == Direction::RIGHT) { rotationOffset = 1; }
			// Bottom wall
			else if (wall == Direction::DOWN) { rotationOffset = 2; }
			// Left wall
			else if (wall == Direction::LEFT) { rotationOffset = 3; }

			// Add texture to quad
			m_Tiles[curVertex + (0 + rotationOffset) % VERTICES_IN_QUAD].texCoords = sf::Vector2f(0.0f, verticalOffset);
			m_Tiles[curVertex + (1 + rotationOffset) % VERTICES_IN_QUAD].texCoords = sf::Vector2f(TILE_SIZE_PIXELS, verticalOffset);
			m_Tiles[curVertex + (2 + rotationOffset) % VERTICES_IN_QUAD].texCoords = sf::Vector2f(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS + verticalOffset);
			m_Tiles[curVertex + (3 + rotationOffset) % VERTICES_IN_QUAD].texCoords = sf::Vector2f(0.0f, TILE_SIZE_PIXELS + verticalOffset);

			// First vertex of next quad
			curVertex += VERTICES_IN_QUAD;
		}
	}
}

// Highlights tile the player is currently on (player can only walk on cell tiles)
void Game::HighlightPlayerTile()
{
	// Previous highlighted tile
	auto oldIndex = m_HighlightedTileIndex;

	// Current highlighted tile
	auto newRow = CalcPlayerRowOnBoard(mp_Player->GetFeetCoordinate());
	auto newCol = CalcPlayerColumnOnBoard(mp_Player->GetPos().x);
	m_HighlightedTileIndex = mp_SudokuBoard->CalcIndex(newRow, newCol);

	// Swap highlighted tile only when it changes
	if (oldIndex != m_HighlightedTileIndex)
	{
		auto oldRow = mp_SudokuBoard->CalcRow(oldIndex);
		auto oldCol = mp_SudokuBoard->CalcColumn(oldIndex);

		// Add wall thickness to translate Sudoku board local row/column to global tile layout row/columnn
		m_TileLayout[oldRow + WALL_THICKNESS_TILES][oldCol + WALL_THICKNESS_TILES] = DEFAULT_CELL_TILE_VALUE;
		m_TileLayout[newRow + WALL_THICKNESS_TILES][newCol + WALL_THICKNESS_TILES] = HIGHLIGHTED_CELL_TILE_VALUE;
	}
}

// Returns tile position of a random incorrect cell (e.g. empty cell or cell with incorrect guess)
sf::Vector2f Game::GetRandomIncorrectCellPos() const
{
	auto index = mp_SudokuBoard->GetRandomIncorrectCellIndex();
	// Add wall thickness to translate Sudoku board local row/column to global tile layout row/columnn
	auto row = mp_SudokuBoard->CalcRow(index) + WALL_THICKNESS_TILES;
	auto col = mp_SudokuBoard->CalcColumn(index) + WALL_THICKNESS_TILES;
	return sf::Vector2f(col + CENTER_ALIGNMENT, row + CENTER_ALIGNMENT) * TILE_SIZE_PIXELS;
}