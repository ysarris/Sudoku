#include "Level_info.h"
#include "TimeManipulation.h"	// for MINUTES
#include "ErrorLogging.h"		// for Assert()
#include "Difficulty_info.h"	// for ValidateDifficulty()

static constexpr auto FIRST_LEVEL = 1;
static constexpr auto FINAL_LEVEL = 10;
static constexpr auto LEVEL_TIME_BASE = (10.0f * MINUTES) + 0.99f; // add 0.99 second delay so the full game time appears longer when the level starts
static constexpr auto LEVEL_TIME_MULTIPLIER = 10.0f * MINUTES;
static constexpr auto AMOUNT_TO_HIDE_BASE = 9;
static constexpr auto AMOUNT_TO_HIDE_DIFFICULTY_MULTIPLIER = 2;
static constexpr auto ERROR_LEVEL_OUT_OF_RANGE = "Level is out of range - value must be between FIRST_LEVEL and FINAL_LEVEL.";

bool IsNewWeaponUnlocked(int level)
{
	ValidateLevel(level);
	return level % 2 != 0;
}

bool IsFinalLevel(int level)
{
	ValidateLevel(level);
	return level == FINAL_LEVEL;
}

int GetFirstLevel()
{
	return FIRST_LEVEL;
}

// Between 9-18, 27-36 or 45-54 hidden values
int GetAmountOfNumbersToHide(int difficulty, int level)
{
	ValidateDifficulty(difficulty);
	ValidateLevel(level);
	return AMOUNT_TO_HIDE_BASE * (1 + difficulty * AMOUNT_TO_HIDE_DIFFICULTY_MULTIPLIER) + (level - 1);
}

// 10, 20 or 30 minutes
float GetLevelTime(int difficulty)
{
	ValidateDifficulty(difficulty);
	return LEVEL_TIME_BASE + (difficulty * LEVEL_TIME_MULTIPLIER);
}

void ValidateLevel(int level)
{
	Assert(ERROR_LEVEL_OUT_OF_RANGE, level >= FIRST_LEVEL && level <= FINAL_LEVEL);
}