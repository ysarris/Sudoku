#include "Game.h"
#include "FileCheck.h"			// for FileExists()
#include "TextCharacters.h"		// for EMPTY_STR, SPACE_CHAR and NEWLINE characters
#include "Highscore_info.h"		// for max score and MAX_HIGHSCORES_STORED
#include "SavedData_info.h"		// for saved data paths
#include <sstream>				// for stringstream
#include <fstream>				// for ifstream, ofstream

static constexpr auto ERROR_NO_GAME_SAVE = "Should not be able to call LoadSave() when no game save exists";

void Game::SaveGame()
{
	// Create string containing all game details
	auto save_details_string = std::to_string(m_Difficulty).append(NEWLINE);
	save_details_string.append(std::to_string(m_Level)).append(NEWLINE);
	save_details_string.append(std::to_string(m_TotalScore)).append(NEWLINE);
	save_details_string.append(mp_Player->GetOwnedWeaponIDs());

	// Replace file contents with string
	std::ofstream gamesaveFile(GetGameSavePath()); // file created if it doesnt exist
	gamesaveFile << save_details_string;
	gamesaveFile.close();
}

void Game::LoadSave()
{
	// File doesnt exist ?
	if (!FileExists(GetGameSavePath()))
		Assert(ERROR_NO_GAME_SAVE);
	// File exists
	else
	{
		std::ifstream gamesaveFile(GetGameSavePath());
		std::stringstream ss;
		std::string line;

		// Extract difficulty
		std::getline(gamesaveFile, line);
		ss << line;
		ss >> m_Difficulty;
		ss.str(EMPTY_STR);
		ss.clear();

		// Extract level
		std::getline(gamesaveFile, line);
		ss << line;
		ss >> m_Level;
		ss.str(EMPTY_STR);
		ss.clear();

		// Extract total score
		std::getline(gamesaveFile, line);
		ss << line;
		ss >> m_TotalScore;
		ss.str(EMPTY_STR);
		ss.clear();

		// Extract weapons
		std::getline(gamesaveFile, line);
		ss << line;
		int weaponID;
		mp_Player->RemoveAllWeapons();
		while (!ss.eof())
		{
			ss >> weaponID;
			mp_Player->AddNewWeapon(weaponID);
			// Dont clear the string between weapons because they're all in one line
		}
		ss.clear(); // Dont clear the error state flags inside loop because we need to know when eof is reached
	}
}

void Game::DeleteSave()
{
	std::remove(GetGameSavePath().c_str()); //get const char* from string
}

void Game::LoadHighscores()
{
	m_Highscores.clear();

	auto highscoresPath = GetHighscoresPath();

	// File doesnt exist ?
	if (!FileExists(highscoresPath))
		std::ofstream highscoresFile{ highscoresPath }; // file created
	else
	{
		std::ifstream highscoresFile(highscoresPath);
		std::stringstream ss;
		std::string line, name;
		int score;

		// Extract each name and score
		while (std::getline(highscoresFile, line))
		{
			ss << line;
			ss >> name >> score;
			ss.str(EMPTY_STR);
			ss.clear();

			m_Highscores.emplace_back(std::make_pair(name, score));
		}
	}
}

// Only called when a new highscore is guaranteed to be entered (list not full or score is higher than the last one)
void Game::SaveNewHighscore(std::string name)
{
	auto savedScores = m_Highscores.size();

	// Exceeded max ?
	if (m_TotalScore > GetMaxScorePossible()) { m_TotalScore = GetMaxScorePossible(); }

	// No scores, just add
	if (savedScores == 0)
		m_Highscores.emplace_back(std::make_pair(name, m_TotalScore));
	// Find where to insert new score
	else
	{
		auto inserted = false;
		auto firstIndex = 0;
		auto index = savedScores - 1;

		// Start comparing from the lowest score, because if new highscore is equal to an existing highscrore the older one has priority
		while (inserted == false)
		{
			// Greater than score at index ?
			if (m_TotalScore > m_Highscores.at(index).second)
			{
				// Only insert here if we reached the highest score, otherwise keep checking
				if (index == firstIndex)
				{
					m_Highscores.insert(m_Highscores.begin() + index, std::make_pair(name, m_TotalScore));
					inserted = true;
				}
			}
			// Not greater than score at index, so insert new score after index
			else
			{
				m_Highscores.insert(m_Highscores.begin() + index + 1, std::make_pair(name, m_TotalScore));
				inserted = true;
			}

			index--;
		}

		// Remove lowest highscore when max exceeded
		if (m_Highscores.size() > MAX_HIGHSCORES_STORED)
			m_Highscores.pop_back();
	}

	// Create string containing all highscores
	std::string highscores_string;
	for (unsigned int i = 0; i < m_Highscores.size(); i++)
	{
		highscores_string.append(m_Highscores.at(i).first).push_back(SPACE_CHAR);
		highscores_string.append(std::to_string(m_Highscores.at(i).second));

		// Dont add new line after last score
		if (i != m_Highscores.size() - 1) { highscores_string.append(NEWLINE); }
	}

	// Replace file contents with string
	std::ofstream highscoresFile(GetHighscoresPath()); // file created if it doesnt exist
	highscoresFile << highscores_string;
	highscoresFile.close();
}