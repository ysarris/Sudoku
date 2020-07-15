#include "LevelEndScreen.h"
#include "StringManipulation.h"		// for FillWithCharacters() and FillWithCharactersInBetween()
#include "Level_info.h"				// for IsFinalLevel() and IsNewWeaponUnlocked()
#include "Highscore_info.h"			// for SCORE_MAX_WIDTH

static constexpr auto LES_CLASS_NAME = "Level End";
static constexpr auto LES_TITLE = "LEVEL ";
static constexpr auto LES_TITLE_FAILED = "FAILED";
static constexpr auto LES_TITLE_COMPLETED = "COMPLETED";
static constexpr auto LES_COMPLETED_MENU_VERTICAL_ALIGNMENT = 0.88f;
static constexpr auto LES_FAILED_MENU_VERTICAL_ALIGNMENT = 0.55f;
static constexpr auto LES_FONT_STYLE = FontStyle::SYMMETRIC;
// Breakdown message
static constexpr auto LES_BREAKDOWN_0 = "POINTS BREAKDOWN";
static constexpr auto LES_BREAKDOWN_1 = "Enemy Kills";
static constexpr auto LES_BREAKDOWN_2 = "Sudoku Solved";
static constexpr auto LES_BREAKDOWN_3 = "Time Remaining";
static constexpr auto LES_BREAKDOWN_4 = "Level Score";
static constexpr auto LES_BREAKDOWN_5 = "TOTAL SCORE:";
static constexpr auto LES_BREAKDOWN_DEFAULT_VERTICAL_ALIGNMENT = 0.32f;
static constexpr auto LES_BREAKDOWN_WITH_SPECIAL_MSG_VERTICAL_ALIGNMENT = 0.28f;
static constexpr auto LES_BREAKDOWN_WIDTH = 16; // enough to fit the longest breakdown string plus 2 spaces
static constexpr auto LES_BREAKDOWN_TEXT_SIZE = 30;
// Special message
static constexpr auto LES_NEW_WEAPON_MSG = "New weapon unlocked!";
static constexpr auto LES_NEW_WEAPON_VERTICAL_ALIGNMENT = 0.77f;
static constexpr auto LES_GAME_COMPLETED_MSG = "Game Completed Well Done!!";
static constexpr auto LES_NEW_HIGHSCORE_MSG = "New Highscore";
static constexpr auto LES_NO_HIGHSCORE_MSG = "No Highscore";
static constexpr auto LES_GAME_COMPLETED_VERTICAL_ALIGNMENT = 0.79f;
static constexpr auto LES_SPECIAL_MSG_WIDTH = 30;
static constexpr auto LES_SPECIAL_MSG_TEXT_SIZE = 25;
static constexpr auto LES_SPECIAL_MSG_OUTLINE_SIZE = 2.0f;
static inline const auto LES_SPECIAL_MSG_COLOR = sf::Color::Blue;
static inline const auto LES_SPECIAL_MSG_OUTLINE_COLOR = sf::Color::White;

static std::string GetScoreBreakdown(int killPoints, int solvedPoints, int timePoints, int totalPoints)
{
	return (FillWithCharacters(LES_BREAKDOWN_0, SPACE_CHAR, FillType::AROUND, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharacters(EMPTY_STR, DASH, FillType::TRAILING, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharactersInBetween(LES_BREAKDOWN_1, std::to_string(killPoints), SPACE_CHAR, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharactersInBetween(LES_BREAKDOWN_2, std::to_string(solvedPoints), SPACE_CHAR, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharactersInBetween(LES_BREAKDOWN_3, std::to_string(timePoints), SPACE_CHAR, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharacters(EMPTY_STR, DASH, FillType::TRAILING, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharactersInBetween(LES_BREAKDOWN_4, std::to_string(killPoints + solvedPoints + timePoints), SPACE_CHAR, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(NEWLINE).append(FillWithCharacters(LES_BREAKDOWN_5, SPACE_CHAR, FillType::AROUND, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH))
		.append(NEWLINE).append(FillWithCharacters(std::to_string(totalPoints), SPACE_CHAR, FillType::AROUND, LES_BREAKDOWN_WIDTH + SCORE_MAX_WIDTH));
}

// Failed
LevelEndScreen::LevelEndScreen(int level) :
	Screen(LES_FAILED_MENU_VERTICAL_ALIGNMENT)
{
	SetTitle(std::string(LES_TITLE).append(std::to_string(level)), LES_TITLE_FAILED);
	m_Menu->AddOption("Try Again", ScreenID::BACK);
	m_Menu->AddOption("Start Screen", ScreenID::START);
}

// Completed
LevelEndScreen::LevelEndScreen(int level, bool isNewHighscore, int killPoints, int solvedPoints, int timePoints, int totalPoints) :
	// For final level move menu lower so it doesnt overlap with the message
	Screen(!IsFinalLevel(level) ? LES_COMPLETED_MENU_VERTICAL_ALIGNMENT : MENU_DEFAULT_VERTICAL_ALIGNMENT)
{
	SetTitle(std::string(LES_TITLE).append(std::to_string(level)), LES_TITLE_COMPLETED);
	m_ScoreBreakdown = GetScoreBreakdown(killPoints, solvedPoints, timePoints, totalPoints);

	if (IsFinalLevel(level))
	{
		m_Menu->AddOption("Next", isNewHighscore ? ScreenID::ENTER_HIGHSCORE_NAME : ScreenID::START);
		m_GameCompleteMessage = FillWithCharacters(LES_GAME_COMPLETED_MSG, SPACE_CHAR, FillType::AROUND, LES_SPECIAL_MSG_WIDTH).append(NEWLINE);
		// New highscore ?
		m_GameCompleteMessage.append(FillWithCharacters(isNewHighscore ? LES_NEW_HIGHSCORE_MSG : LES_NO_HIGHSCORE_MSG, SPACE_CHAR, FillType::AROUND, LES_SPECIAL_MSG_WIDTH));
	}
	else
	{
		m_Menu->AddOption("Next Level", ScreenID::BACK);
		m_Menu->AddOption("Start Screen", ScreenID::START);
		// Unlocked new weapon ?
		if (IsNewWeaponUnlocked(level)) { m_NewWeaponMessage = LES_NEW_WEAPON_MSG; }
	}
}

ScreenID LevelEndScreen::GetID() const
{
	return ScreenID::LEVEL_END;
}

void LevelEndScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Default screen draw
	Screen::Draw(rWindow);

	// Completed ?
	if (m_ScoreBreakdown.size() != 0)
	{
		auto breakdownVerticalAlignment = LES_BREAKDOWN_DEFAULT_VERTICAL_ALIGNMENT;

		// Final level ?
		if (m_GameCompleteMessage.size() != 0)
		{
			// Move breakdown higher so it doesnt overlap with message
			breakdownVerticalAlignment = LES_BREAKDOWN_WITH_SPECIAL_MSG_VERTICAL_ALIGNMENT;
			PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, LES_GAME_COMPLETED_VERTICAL_ALIGNMENT), m_GameCompleteMessage,
				LES_SPECIAL_MSG_TEXT_SIZE, LES_SPECIAL_MSG_COLOR, LES_SPECIAL_MSG_OUTLINE_COLOR, LES_SPECIAL_MSG_OUTLINE_SIZE, LES_FONT_STYLE);
		}
		// Unlocked new weapon ?
		else if (m_NewWeaponMessage.size() != 0)
		{
			// Move breakdown higher so it doesnt overlap with message
			breakdownVerticalAlignment = LES_BREAKDOWN_WITH_SPECIAL_MSG_VERTICAL_ALIGNMENT;
			PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, LES_NEW_WEAPON_VERTICAL_ALIGNMENT), m_NewWeaponMessage,
				LES_SPECIAL_MSG_TEXT_SIZE, LES_SPECIAL_MSG_COLOR, LES_SPECIAL_MSG_OUTLINE_COLOR, LES_SPECIAL_MSG_OUTLINE_SIZE, LES_FONT_STYLE);
		}

		// Score breakdown
		PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, breakdownVerticalAlignment), m_ScoreBreakdown,
			LES_BREAKDOWN_TEXT_SIZE, DEFAULT_TEXT_COLOR, LES_FONT_STYLE);
	}
}

const std::string LevelEndScreen::GetClassName() const
{
	return LES_CLASS_NAME;
}