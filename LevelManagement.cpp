#include "Game.h"
#include "Pistol.h"			// for PISTOL_ID
#include "NewGameScreen.h"	// for dynamic_cast<NewGameScreen&> to get difficulty
#include "Level_info.h"		// for GetFirstLevel(), GetLevelTime() and GetAmountOfNumbersToHide()
#include "Spawn_info.h"		// for spawn functions

static constexpr auto DEFAULT_SCORE = 0;
static constexpr auto STARTER_WEAPON_ID = PISTOL_ID;

void Game::NewGame()
{
	// Get chosen difficulty
	m_Difficulty = dynamic_cast<NewGameScreen&>(*mp_CurrentScreen).GetDifficulty();
	m_Level = GetFirstLevel();
	m_TotalScore = DEFAULT_SCORE;
	mp_Player->ResetNewGame(STARTER_WEAPON_ID);
	SaveGame();
}

void Game::CleanupLevel()
{
	// Stop all game object sounds
	mp_Player->Pause();
	for (auto& gate : m_Gates) { gate->Pause(); }
	// Clear objects no longer needed
	m_Enemies.clear();
	m_Projectiles.clear();
	m_Collectables.clear();
}

void Game::PrepareLevel()
{
	// RESET
	// Game objects
	mp_Player->ResetNewLevel();
	Gate::ResetGates();
	// Scores
	m_KillsScore = DEFAULT_SCORE;
	m_SolutionScore = DEFAULT_SCORE;
	m_TimeScore = DEFAULT_SCORE;
	// Timers and Counts
	m_TimeRemaining.Reset(GetLevelTime(m_Difficulty));
	m_HealthPackSpawnTimer.Reset(GetHealthPackSpawnTime(m_Difficulty));
	m_SmallEnemySpawnTimer.Reset(GetFirstEnemyWaveSpawnTime());
	m_LargeEnemySpawnCount = 0;
	// Sudoku board
	m_SudokuCompleted = false;
	mp_SudokuBoard->GenerateNewSudoku();
	mp_SudokuBoard->HideCells(GetAmountOfNumbersToHide(m_Difficulty, m_Level));
}