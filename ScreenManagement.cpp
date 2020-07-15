#include "Game.h"
#include "StartScreen.h"		// for screen IDs and classes
#include "GameScreen.h"
#include "NewGameScreen.h"
#include "HighscoresScreen.h"
#include "TutorialScreen.h"
#include "PauseScreen.h"
#include "LevelEndScreen.h"
#include "NameEntryScreen.h"
#include "QuitScreen.h"
#include "FileCheck.h"			// for FileExists()
#include "TimeManipulation.h"	// for GetMinutes()
#include "Level_info.h"			// for IsFinalLevel() and IsNewWeaponUnlocked()
#include "Scores_info.h"		// for CalcSolutionScore() and CalcTimeScore()
#include "SavedData_info.h"		// for GetGameSavePath()
#include "Highscore_info.h"		// for MAX_HIGHSCORES_STORED

static constexpr auto SELECT_SOUND_FILENAME = "select";

static bool IsNewHighscore(int scoreToCheck, std::vector <std::pair<std::string, int>>& rHighscores)
{
	return (rHighscores.size() < MAX_HIGHSCORES_STORED) || (scoreToCheck > rHighscores.back().second);
}

void Game::SwitchScreen(ScreenID newScreenID)
{
	SoundHolder::Play(mp_SelectSound, SoundHolder::GetPath(SELECT_SOUND_FILENAME));

	// Store old ID temporarily
	auto oldScreenID = mp_CurrentScreen->GetID();

	switch (newScreenID)
	{
	case ScreenID::START:
		// Abandoned level from pause screen ?
		if (oldScreenID == ScreenID::PAUSE) { CleanupLevel(); }
		// Clear any screens no longer needed in memory
		// Avoids creating multiple copies of screens from loops (e.g. start->game->pause->abandon)
		m_Screens.clear();
		m_Screens.emplace_back(std::make_unique<StartScreen>(FileExists(GetGameSavePath())));
		break;
	case ScreenID::NEW_GAME:
		m_Screens.emplace_back(std::make_unique<NewGameScreen>());
		break;
	case ScreenID::GAME:
		// Continue
		if (oldScreenID == ScreenID::START) { LoadSave(); }
		// New game
		else if (oldScreenID == ScreenID::NEW_GAME) { NewGame(); }
		// Clear any screens no longer needed in memory
		// Avoid creating multiple copies of screens from loops (e.g. start->new game->game->pause->abandon)
		m_Screens.clear();
		m_Screens.emplace_back(std::make_unique<GameScreen>(m_Difficulty, m_Level, m_TimeRemaining, *mp_SudokuBoard, *mp_Player, m_KillsScore, m_HighlightedTileIndex));
		PrepareLevel();
		break;
	case ScreenID::PAUSE:
		Pause();
		m_Screens.emplace_back(std::make_unique<PauseScreen>());
		break;
	case ScreenID::LEVEL_END:
		CleanupLevel();
		// Level failed ?
		if (!m_SudokuCompleted)
			m_Screens.emplace_back(std::make_unique<LevelEndScreen>(m_Level));
		// Level completed
		else
		{
			// Calculate scores
			m_SolutionScore = CalcSolutionScore(m_Difficulty, m_Level);
			m_TimeScore = CalcTimeScore(m_Difficulty, m_Level, GetMinutes(m_TimeRemaining.GetTimeLeft()));
			m_TotalScore += m_KillsScore + m_SolutionScore + m_TimeScore;

			// Unlocked new weapon ?
			if (IsNewWeaponUnlocked(m_Level)) { mp_Player->AddRandomNewWeapon(); }

			m_Screens.emplace_back(std::make_unique<LevelEndScreen>(m_Level, IsNewHighscore(m_TotalScore, m_Highscores), m_KillsScore, m_SolutionScore, m_TimeScore, m_TotalScore));

			// Save current progress, only allow final level to be replayed for chance at highscore
			if (!IsFinalLevel(m_Level))
			{
				m_Level++;
				SaveGame();
			}
		}
		break;
	case ScreenID::TUTORIAL:
		m_Screens.emplace_back(std::make_unique<TutorialScreen>());
		break;
	case ScreenID::HIGHSCORES:
	{
		auto newScoreJustEntered = false;
		// Was a new score just entered?
		if (oldScreenID == ScreenID::ENTER_HIGHSCORE_NAME)
		{
			newScoreJustEntered = true;
			SaveNewHighscore(dynamic_cast<NameEntryScreen&>(*mp_CurrentScreen).GetNameEntered());
			DeleteSave(); // so "Continue" doesnt appear in start menu
		}
		m_Screens.emplace_back(std::make_unique<HighscoresScreen>(m_Highscores, newScoreJustEntered));
	}
	break;
	case ScreenID::ENTER_HIGHSCORE_NAME:
		m_Screens.emplace_back(std::make_unique<NameEntryScreen>());
		break;
	case ScreenID::QUIT:
		// Attempting to quit from game screen ?
		if (oldScreenID == ScreenID::GAME) { Pause(); }
		m_Screens.emplace_back(std::make_unique<QuitScreen>());
		break;
	case ScreenID::CLOSED:
		Close();
		return;
		break;
	case ScreenID::BACK:
		// Remove last screen
		m_Screens.pop_back();

		// Back into game ?
		if (m_Screens.back()->GetID() == ScreenID::GAME)
		{
			// From level end ?
			if (oldScreenID == ScreenID::LEVEL_END) { PrepareLevel(); }
			// From pause/quit ?
			else if (oldScreenID == ScreenID::PAUSE || oldScreenID == ScreenID::QUIT) { Resume(); }
		}
		break;
	default:
		Assert(ERROR_UNKNOWN_SCREEN_ID);
		break;
	}

	// Make the last screen in vector the current screen
	mp_CurrentScreen = m_Screens.back().get();
}