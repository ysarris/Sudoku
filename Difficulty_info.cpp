#include "Difficulty_info.h"
#include "ErrorLogging.h"		// for Assert()

static constexpr auto MIN_DIFFICULTY = 0;
static constexpr auto MAX_DIFFICULTY = 2;
static constexpr auto DEFAULT_DIFFICULTY = 1;
static constexpr auto ERROR_DIFFICULTY_OUT_OF_RANGE = "Difficulty is out of range - value must be between 0 and GetNumberOfDifficulties() - 1.";

std::string GetDifficultyString(int difficulty)
{
	switch (difficulty)
	{
	case 0:
		return "EASY";
		break;
	case 1:
		return "NORMAL";
		break;
	case 2:
		return "HARD";
		break;
	default:
		Assert(ERROR_DIFFICULTY_OUT_OF_RANGE);
		break;
	}
}

int GetDefaultDifficulty()
{
	return DEFAULT_DIFFICULTY;
}

int GetNumberOfDifficulties()
{
	return MAX_DIFFICULTY - MIN_DIFFICULTY + 1;
}

void ValidateDifficulty(int difficulty)
{
	Assert(ERROR_DIFFICULTY_OUT_OF_RANGE, difficulty >= 0 && difficulty <= GetNumberOfDifficulties() - 1);
}