#include "Game.h"
#include "Tile_info.h"		// for TILE_SIZE_PIXELS
#include "Walls_info.h"		// for WALL_THICKNESS

static constexpr auto BOARD_FIRST_CELL_POSX = (WALL_THICKNESS_TILES + 0.5f) * TILE_SIZE_PIXELS;
static constexpr auto BOARD_FIRST_CELL_POSY = BOARD_FIRST_CELL_POSX;

void Game::Draw()
{
	m_Window.clear();

	// In game ?
	if (mp_CurrentScreen->GetID() == ScreenID::GAME)
	{
		// Draw tiles and board
		m_Window.draw(m_Tiles, &m_TileTextures);
		mp_SudokuBoard->Draw(m_Window, TILE_SIZE_PIXELS, sf::Vector2f(BOARD_FIRST_CELL_POSX, BOARD_FIRST_CELL_POSY));

		// Draw enemies that are not out of the gates first
		for (auto& curEnemy : m_Enemies) { if (!curEnemy->IsOutOfGate()) { curEnemy->Draw(m_Window); } }

		// Draw gates
		for (auto& curGate : m_Gates) { curGate->Draw(m_Window); }

		// Draw enemies that are out of the gates next
		for (auto& curEnemy : m_Enemies) { if (curEnemy->IsOutOfGate()) { curEnemy->Draw(m_Window); } }

		// Draw non-alive non-explosive projectiles in front of enemies and behind player
		// (e.g. hatchet stuck in enemy/floor, bullet blood splat on enemy)
		for (auto& curProjectile : m_Projectiles)
			if (curProjectile->GetStatus() != HealthStatus::ALIVE && !curProjectile->IsExploding()) { curProjectile->Draw(m_Window); }

		// Draw player
		mp_Player->Draw(m_Window);

		// Draw alive projectiles or exploding projectiles in front of enemies and player
		// (e.g. hatchet/grenade in air, grenade explosion)
		for (auto& curProjectile : m_Projectiles)
			if (curProjectile->GetStatus() == HealthStatus::ALIVE || curProjectile->IsExploding()) { curProjectile->Draw(m_Window); }

		// Draw collectables
		for (auto& curCollectable : m_Collectables) { curCollectable->Draw(m_Window); }
	}

	switch (mp_CurrentScreen->GetID())
	{
	// Drawabe screens
	case ScreenID::START:
	case ScreenID::NEW_GAME:
	case ScreenID::GAME: // draws the HUD in front of everything else
	case ScreenID::PAUSE:
	case ScreenID::LEVEL_END:
	case ScreenID::TUTORIAL:
	case ScreenID::HIGHSCORES:
	case ScreenID::ENTER_HIGHSCORE_NAME:
	case ScreenID::QUIT:
		mp_CurrentScreen->Draw(m_Window);
		break;
	// Not drawable
	case ScreenID::CLOSED:
	case ScreenID::BACK:
		break;
	default:
		Assert(ERROR_UNKNOWN_SCREEN_ID);
		break;
	}

	// Display everything drawn
	m_Window.display();
}