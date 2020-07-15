#include "Scores_info.h"
#include "ErrorLogging.h"		// for Assert()
#include "Difficulty_info.h"	// for ValidateDifficulty();
#include "Level_info.h"			// for ValidateLevel() and GetLevelTime()

static constexpr auto SUDOKU_SOLUTION_POINTS_BASE = 500;
static constexpr auto TIME_LEFT_POINTS_BASE = 100;
static constexpr auto TIME_LEFT_MULTIPLIER = 5;
static constexpr auto ENEMY_KILL_MULTIPLIER = 2;
static constexpr auto ERROR_MINUTES_REMAINING_OUT_OF_RANGE = "Minutes remaining value is out of range - value must be between zero and GetLevelTime().";
static constexpr auto ERROR_ENEMY_KILL_POINTS_INVALID = "Enemy kill points value is invalid - value must be greater than zero.";

// level * 500, 1000 or 1500 points
int CalcSolutionScore(int difficulty, int level)
{
	ValidateDifficulty(difficulty);
	ValidateLevel(level);
	return (difficulty + 1) * level * SUDOKU_SOLUTION_POINTS_BASE;
}

// level * 100, 500 or 1000 points (for each full minute remaining)
int CalcTimeScore(int difficulty, int level, int minutesRemaining)
{
	ValidateDifficulty(difficulty);
	ValidateLevel(level);
	Assert(ERROR_MINUTES_REMAINING_OUT_OF_RANGE, minutesRemaining >= 0 && minutesRemaining < GetLevelTime(difficulty));
	return (difficulty == 0 ? 1 : TIME_LEFT_MULTIPLIER * difficulty) * minutesRemaining * level * TIME_LEFT_POINTS_BASE;
}

// 1, 2 or 4 * enemyKillPoints
int CalcKillPoints(int difficulty, int enemyKillPoints)
{
	ValidateDifficulty(difficulty);
	Assert(ERROR_ENEMY_KILL_POINTS_INVALID, enemyKillPoints > 0);
	return (difficulty == 0 ? 1 : ENEMY_KILL_MULTIPLIER * difficulty) * enemyKillPoints;
}