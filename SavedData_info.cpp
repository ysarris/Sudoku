#include "SavedData_info.h"

static constexpr auto SAVEDATA_DIRECTORY = "Savedata/";
static constexpr auto GAMESAVE_FILENAME = "Gamesave.txt";
static constexpr auto HIGHSCORES_FILENAME = "Highscores.txt";

std::string GetGameSavePath()
{
	return std::string(SAVEDATA_DIRECTORY).append(GAMESAVE_FILENAME);
}

std::string GetHighscoresPath()
{
	return std::string(SAVEDATA_DIRECTORY).append(HIGHSCORES_FILENAME);
}