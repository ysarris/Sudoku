#include "Highscore_info.h"
#include "StringManipulation.h"		// for FillWithCharacters()

int GetMaxScorePossible()
{
	return std::stoi(FillWithCharacters(EMPTY_STR, '9', FillType::LEADING, SCORE_MAX_WIDTH));
}