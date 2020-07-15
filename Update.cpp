#include "Game.h"
#include "HealthPack.h"
#include "Random.h"			// for GetRandomNumberBetween()
#include "Enemy_info.h"		// for enemy classes, MIN_ENEMY_WAVE_SIZE and MAX_ENEMY_WAVE_SIZE
#include "Spawn_info.h"		// for spawn functions

static constexpr auto WIN_SOUND_FILENAME = "win";
static constexpr auto LOSE_SOUND_FILENAME = "lose";
static constexpr auto DELTA_TIME_MAX = 1.0f / WINDOW_FRAME_RATE_LIMIT;

void Game::Update(float dtAsSeconds)
{
	// Too long passed since last frame ? Limit the delta time
	if (dtAsSeconds > DELTA_TIME_MAX) { dtAsSeconds = DELTA_TIME_MAX; }

	switch (mp_CurrentScreen->GetID())
	{
	// Updatable
	case ScreenID::START:
	case ScreenID::NEW_GAME:
	case ScreenID::GAME:
	case ScreenID::PAUSE:
	case ScreenID::LEVEL_END:
	case ScreenID::TUTORIAL:
	case ScreenID::HIGHSCORES:
	case ScreenID::ENTER_HIGHSCORE_NAME:
	case ScreenID::QUIT:
		mp_CurrentScreen->Update(dtAsSeconds);
		break;
	// Not updatable
	case ScreenID::CLOSED:
	case ScreenID::BACK:
		break;
	default:
		Assert(ERROR_UNKNOWN_SCREEN_ID);
		break;
	}

	// In game ?
	if (mp_CurrentScreen->GetID() == ScreenID::GAME)
	{
		auto alreadyWon = HasWon();
		auto alreadyLost = HasLost();
		auto gatesOpen = Gate::AreGatesOpen();

		// TIME TO SPAWN ?
		// Health pack
		if (m_HealthPackSpawnTimer.RanOut())
		{
			m_Collectables.emplace_back(std::make_unique<HealthPack>(*mp_Player));
			m_HealthPackSpawnTimer.Reset(GetHealthPackSpawnTime(m_Difficulty));
		}
		else { m_HealthPackSpawnTimer.Decrement(dtAsSeconds); }
		// Small enemies
		if (gatesOpen)
		{
			if (m_SmallEnemySpawnTimer.RanOut())
			{
				m_SmallEnemySpawnTimer.Reset(GetNextEnemyWaveSpawnTime());

				// Spawn random sized wave from each gate
				for (const auto& gate : m_Gates)
					for (auto i = 0; i < GetRandomNumberBetween(MIN_ENEMY_WAVE_SIZE, MAX_ENEMY_WAVE_SIZE); i++)
						m_Enemies.emplace_back(std::make_unique<Wanderer>(gate->GetPos(), gate->GetExitFacing()));
			}
			else { m_SmallEnemySpawnTimer.Decrement(dtAsSeconds); }
		}
		// Large enemy
		if (m_LargeEnemySpawnCount < GetLargeEnemySpawnLimit(m_Difficulty)
			&& mp_SudokuBoard->GetIncorrectCellCount() == GetEmptyCellsForLargeEnemySpawn(m_Difficulty, m_Level, m_LargeEnemySpawnCount))
		{
			// Spawn from a tile with an incorrect guess or no guess
			m_Enemies.emplace_back(std::make_unique<Obstructer>(GetRandomIncorrectCellPos()));
			m_LargeEnemySpawnCount++;
		}

		// Update all game objects
		mp_Player->Update(dtAsSeconds);
		Gate::UpdateGates(dtAsSeconds);

		// Gates were open before updating but not after ? Prepare timer for next time gates open
		if (gatesOpen && !Gate::AreGatesOpen()) { m_SmallEnemySpawnTimer.Reset(GetFirstEnemyWaveSpawnTime()); }

		for (auto enemy_it = m_Enemies.begin(); enemy_it != m_Enemies.end();)
		{
			auto& enemy = *enemy_it;

			// Delete dead enemies
			if (enemy->GetStatus() == HealthStatus::DEAD)
				enemy_it = m_Enemies.erase(enemy_it);
			// Update the rest
			else
			{
				enemy->Update(dtAsSeconds);
				enemy_it++;
			}
		}

		for (auto projectile_it = m_Projectiles.begin(); projectile_it != m_Projectiles.end();)
		{
			auto& projectile = *projectile_it;

			// Delete dead projectiles
			if (projectile->GetStatus() == HealthStatus::DEAD)
				projectile_it = m_Projectiles.erase(projectile_it);
			// Update the rest
			else
			{
				projectile->Update(dtAsSeconds);
				projectile_it++;
			}
		}

		for (auto collectable_it = m_Collectables.begin(); collectable_it != m_Collectables.end();)
		{
			auto& collectable = *collectable_it;

			// Delete collected or despawned collectables that finished playing their collect/despawn sound 
			if ((collectable->IsCollected() || collectable->IsDespawned()) && !collectable->SoundIsPlaying())
				collectable_it = m_Collectables.erase(collectable_it);
			// Update the rest
			else
			{
				collectable->Update(dtAsSeconds);
				collectable_it++;
			}
		}

		// Manage collisions between all game objects
		DetectCollisions();

		// Update tiles
		HighlightPlayerTile();
		SetTVAfromTLA();

		// Havent won already ?
		if (!alreadyWon)
		{
			m_SudokuCompleted = mp_SudokuBoard->IsSolutionCorrect();

			// Won now ?
			if (HasWon()) { SoundHolder::Play(mp_LevelEndSound, SoundHolder::GetPath(WIN_SOUND_FILENAME)); }
			// Havent lost already ?
			else if (!alreadyLost)
			{
				m_TimeRemaining.Decrement(dtAsSeconds);

				// Lost now ?
				if (HasLost()) { SoundHolder::Play(mp_LevelEndSound, SoundHolder::GetPath(LOSE_SOUND_FILENAME)); }
			}
		}

		// Level over ?
		if ((HasWon() || HasLost()) && mp_LevelEndSound->getStatus() != sf::Sound::Playing) { SwitchScreen(ScreenID::LEVEL_END); }
	}
}