#include "Spawn_info.h"
#include "TimeManipulation.h"	// for MINUTES
#include "ErrorLogging.h"		// for Assert()
#include "Difficulty_info.h"	// for ValidateDifficulty();
#include "Level_info.h"			// for ValidateLevel() and GetAmountOfNumbersToHide()

static constexpr auto HEALTH_PACK_SPAWN_TIME_BASE = 1.5f * MINUTES;
static constexpr auto HEALTH_PACK_SPAWN_TIME_MULTIPLIER = 3.0f * MINUTES;
static constexpr auto TIME_UNTIL_FIRST_ENEMY_WAVE = 1.0f;
static constexpr auto TIME_UNTIL_NEXT_ENEMY_WAVE = 5.0f;
static constexpr auto ERROR_SPAWNED_ALL_LARGE_ENEMIES = "Already spawned all large enemies allowed - cant spawn more enemies than GetLargeEnemySpawnLimit().";

// 1.5, 4.5 or 7.5 minutes
float GetHealthPackSpawnTime(int difficulty)
{
	ValidateDifficulty(difficulty);
	return HEALTH_PACK_SPAWN_TIME_BASE + (difficulty * HEALTH_PACK_SPAWN_TIME_MULTIPLIER);
}

// 1, 2 or 3
int GetLargeEnemySpawnLimit(int difficulty)
{
	ValidateDifficulty(difficulty);
	return difficulty + 1;
}

// Half of the initial hidden cells for given level of each difficulty
int GetEmptyCellsForLargeEnemySpawn(int difficulty, int level, int alreadySpawned)
{
	ValidateDifficulty(difficulty);
	ValidateLevel(level);
	Assert(ERROR_SPAWNED_ALL_LARGE_ENEMIES, alreadySpawned < GetLargeEnemySpawnLimit(difficulty));
	return static_cast<int>(GetAmountOfNumbersToHide(difficulty - alreadySpawned, level) / 2);
}

float GetFirstEnemyWaveSpawnTime()
{
	return TIME_UNTIL_FIRST_ENEMY_WAVE;
}

float GetNextEnemyWaveSpawnTime()
{
	return TIME_UNTIL_NEXT_ENEMY_WAVE;
}