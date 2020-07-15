#include "Board.h"
#include "Random.h"								// for GetRandomNumberBetween()
#include "TextPrinting.h"						// for GetFontPath() and PrintText()
#include "ErrorLogging.h"						// for Assert()
#include <SFML/Graphics/RectangleShape.hpp>		// for gridlines

static constexpr auto CELL_FONT_FILENAME = "Opensticks";
static constexpr auto CELL_TEXT_SIZE = 40;
static constexpr auto CELL_TEXT_STYLE = TextStyle::BOLD;
static inline const auto CELL_TEXT_COLOR_HIDDEN = sf::Color::White;
static inline const auto CELL_TEXT_COLOR_NOT_HIDDEN = sf::Color(0, 0, 0, 200);
static inline const auto GRIDLINE_COLOR = sf::Color::Black;
static constexpr auto GRIDLINE_THICKNESS = 1.0f;
static constexpr auto ERROR_CELL_OUT_OF_RANGE = "Cell index is out of range - value must be between 0 and 80.";
static constexpr auto ERROR_ROW_OUT_OF_RANGE = "Row index is out of range - value must be between 0 and 8.";
static constexpr auto ERROR_COLUMN_OUT_OF_RANGE = "Column index is out of range - value must be between 0 and 8.";
static constexpr auto ERROR_AMOUNT_TO_HIDE_OUT_OF_RANGE = "Amount of cells to hide is out of range - value must be between 1 and 81.";
static constexpr auto ERROR_AMOUNT_TO_HIDE_GREATER_THAN_CELLS_NOT_HIDDEN = "Amount of cells to hide is greater than remaining cells not hidden - use value <= to GetAmountOfCellsNotHidden() return value.";
static constexpr auto ERROR_NO_INCORRECT_CELLS = "No more incorrect cells, sudoku has been solved - use IsSolutionCorrect() to check before calling GetRandomIncorrectCellIndex().";

static int CalcBox(int index)
{
	return index / BOARD_SIZE;
}

static int CalcBox(int row, int column)
{
	return (row / BOX_SIZE) * BOX_SIZE + (column / BOX_SIZE);
}

static void PrintGridline(sf::RenderWindow& rWindow, sf::Vector2f position, sf::Vector2f dimensions)
{
	sf::RectangleShape gridLine;
	gridLine.setFillColor(GRIDLINE_COLOR);
	gridLine.setPosition(position);
	gridLine.setSize(dimensions);
	rWindow.draw(gridLine);
}

Board::Board() :
	m_SudokuCells(),
	m_HiddenCellCount(),
	m_EmptyCellCount(),
	m_CorrectCellCount()
{
	ms_CellsFont.loadFromFile(GetFontPath(CELL_FONT_FILENAME));
}

void Board::GenerateNewSudoku()
{
	m_SudokuCells.clear();
	m_SudokuCells.resize(BOARD_CELLS);

	// Loop through all cells (0 to 80)
	for (auto cell_it = m_SudokuCells.begin(); cell_it != m_SudokuCells.end(); cell_it++)
	{
		auto validValue = false;

		// Found a valid value yet ?
		while (!validValue)
		{
			// Calculate current cell index
			// Needs to be calculculated inside the loop in case we backtrack
			auto index = distance(m_SudokuCells.begin(), cell_it);
			auto& cell = m_SudokuCells.at(index);

			// No more value candidates to try ?
			if (cell.RanOutOfCandidates())
			{
				cell.Reset();
				// Backtrack to previous cell
				cell_it--;
			}
			// Try next random candidate
			else
			{
				auto candidate = cell.GetRandomAvailableCandidate();
				// Would candidate be valid in column/row/box ?
				validValue = IsColumnValid(candidate, index) && IsRowValid(candidate, index) && IsBoxValid(candidate, index);
				if (validValue) { cell.SetValue(candidate); }
			}
		}
	}

	// Reset counts
	m_HiddenCellCount = 0;
	m_EmptyCellCount = 0;
	m_CorrectCellCount = BOARD_CELLS;
}

void Board::HideCells(int amountToHide)
{
	Assert(ERROR_AMOUNT_TO_HIDE_OUT_OF_RANGE, amountToHide > 0 && amountToHide <= BOARD_CELLS);
	Assert(ERROR_AMOUNT_TO_HIDE_GREATER_THAN_CELLS_NOT_HIDDEN, amountToHide <= GetVisibleCellCount());
	auto hiddenSoFar = 0;
	int index;
	// Populate a set with all indeces
	std::set<int> indeces;
	for (auto i = 0; i < static_cast<int>(m_SudokuCells.size()) - 1; i++) { indeces.insert(i); }

	// Hidden enough cells yet ?
	while (hiddenSoFar < amountToHide)
	{
		// Pick a random cell
		index = TakeRandomElementFromSet(indeces);

		// Already hidden ?
		if (!m_SudokuCells.at(index).IsHidden())
		{
			m_SudokuCells.at(index).Hide();
			hiddenSoFar++;
			m_HiddenCellCount++;
			m_EmptyCellCount++;
			m_CorrectCellCount--;
		}
	}
}

void Board::ClearCellGuess(int index)
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_CELLS);
	if (!IsCellEmpty(index)) { m_EmptyCellCount++; } // dont decrement if cell was already empty
	if (m_SudokuCells.at(index).IsGuessCorrect()) { m_CorrectCellCount--; } // decrement if correct guess was cleared
	m_SudokuCells.at(index).ClearGuess();
}

void Board::EnterCellGuess(int index, int value)
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_CELLS);
	if (IsCellEmpty(index)) { m_EmptyCellCount--; } // dont decrement if cell already had another guess
	if (m_SudokuCells.at(index).IsGuessCorrect()) { m_CorrectCellCount--; } // decrement if correct guess was changed
	m_SudokuCells.at(index).SetGuess(value);
	if (m_SudokuCells.at(index).IsGuessCorrect()) { m_CorrectCellCount++; } // increment if new guess is correct
}

bool Board::IsSolutionCorrect() const
{
	return m_CorrectCellCount == BOARD_CELLS;
}

bool Board::IsCellHidden(int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_CELLS);
	return m_SudokuCells.at(index).IsHidden();
}

bool Board::IsCellEmpty(int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_CELLS);
	return m_SudokuCells.at(index).IsGuessClear();
}

int Board::CalcIndex(int row, int column) const
{
	Assert(ERROR_ROW_OUT_OF_RANGE, row >=0 && row  < BOARD_SIZE);
	Assert(ERROR_COLUMN_OUT_OF_RANGE, column >=0 && column < BOARD_SIZE);
	auto box = CalcBox(row, column);
	auto index = (BOX_SIZE * row) + column + box * (BOARD_SIZE - BOX_SIZE);
	return index;
}

int Board::CalcRow(int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_CELLS);
	auto box = CalcBox(index);
	auto withinBoxRow = (index % BOARD_SIZE) / BOX_SIZE;
	auto additionalRows = (box / BOX_SIZE) * BOX_SIZE;
	return withinBoxRow + additionalRows;
}

int Board::CalcColumn(int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_CELLS);
	auto box = CalcBox(index);
	auto withinBoxColumn = index % BOX_SIZE;
	auto additionalColumns = (box % BOX_SIZE) * BOX_SIZE;
	return withinBoxColumn + additionalColumns;
}

// Cells with wrong a guess and empty cells are considered incorrect
int Board::GetRandomIncorrectCellIndex() const
{
	Assert(ERROR_NO_INCORRECT_CELLS, !IsSolutionCorrect());
	int index;
	// Populate a set with all indeces
	std::set<int> indeces;
	for (auto i = 0; i < static_cast<int>(m_SudokuCells.size()) - 1; i++) { indeces.insert(i); }
	
	do
	{
		// Pick a random cell
		index = TakeRandomElementFromSet(indeces);
	}
	while (m_SudokuCells.at(index).IsGuessCorrect());

	return index;
}

int Board::GetVisibleCellCount() const
{
	return BOARD_CELLS - m_HiddenCellCount;
}

int Board::GetIncorrectCellCount() const
{
	return BOARD_CELLS - m_CorrectCellCount;
}

void Board::Draw(sf::RenderWindow& rWindow, float cellSize, sf::Vector2f firstCellPos) const
{
	sf::Color color;
	sf::Vector2f alignment;

	for (auto r = 0; r < BOARD_SIZE; r++)
	{
		for (auto c = 0; c < BOARD_SIZE; c++)
		{
			// Numbers
			auto& curCell = m_SudokuCells.at(CalcIndex(r, c));
			if (!curCell.IsGuessClear())
			{
				color = (curCell.IsHidden()) ? CELL_TEXT_COLOR_HIDDEN : CELL_TEXT_COLOR_NOT_HIDDEN;
				alignment = sf::Vector2f((firstCellPos.x + c * cellSize) / rWindow.getSize().x, (firstCellPos.y + r * cellSize) / rWindow.getSize().y);
				PrintText(rWindow, alignment, std::to_string(curCell.GetGuess()), CELL_TEXT_SIZE, color, ms_CellsFont, CELL_TEXT_STYLE);
			}
			// Gridlines
			// Horizontal
			if (c == 0 && r != 0 && r % BOX_SIZE == 0)
				PrintGridline(rWindow, sf::Vector2f(firstCellPos.x - cellSize * 0.5f, firstCellPos.y + cellSize * (r - 0.5f)), sf::Vector2f(BOARD_SIZE * cellSize, GRIDLINE_THICKNESS));
			// Vertical
			else if (r == 0 && c != 0 && c % BOX_SIZE == 0)
				PrintGridline(rWindow, sf::Vector2f(firstCellPos.x + cellSize * (c - 0.5f), firstCellPos.y - cellSize * 0.5f), sf::Vector2f(GRIDLINE_THICKNESS, BOARD_SIZE * cellSize));
		}
	}
}

bool Board::IsBoxValid(int value, int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_SIZE* BOARD_SIZE);
	auto box = CalcBox(index);

	// Check all cells in that box
	for (auto i = 0 + (box * BOARD_SIZE); i < BOARD_SIZE + (box * BOARD_SIZE); i++)
	{
		// Value already exists in box ?
		if (m_SudokuCells.at(i).GetValue() == value) { return false; }
	}
	return true;
}

bool Board::IsRowValid(int value, int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_SIZE* BOARD_SIZE);
	auto row = CalcRow(index);

	// Check all cells in that row
	for (auto c = 0; c < BOARD_SIZE; c++)
	{
		// Value already exists in row ?
		if (m_SudokuCells.at(CalcIndex(row, c)).GetValue() == value) { return false; }
	}
	return true;
}

bool Board::IsColumnValid(int value, int index) const
{
	Assert(ERROR_CELL_OUT_OF_RANGE, index >= 0 && index < BOARD_SIZE* BOARD_SIZE);
	auto column = CalcColumn(index);

	// Check all cells in that column
	for (auto r = 0; r < BOARD_SIZE; r++)
	{
		// Value already exists in column ?
		if (m_SudokuCells.at(CalcIndex(r, column)).GetValue() == value) { return false; }
	}
	return true;
}