#include "GameScreen.h"
#include "GameplayControls.h"		// for Player input functions
#include "StringManipulation.h"		// for FillWithCharacters(), also includes "TextCharacters.h"
#include "TimeManipulation.h"		// for GetMinutes() and GetSeconds()
#include "Tile_info.h"				// for TILE_SIZE
#include "Highscore_info.h"			// for SCORE_MAX_WIDTH
#include "Difficulty_info.h"		// for GetDifficultyString()

static constexpr auto GAME_SCREEN_CLASS_NAME = "Game";
// Input sounds
static constexpr auto GAME_SCREEN_ENTER_SOUND_FILENAME = "Enter_Number";
static constexpr auto GAME_SCREEN_CLEAR_SOUND_FILENAME = "Clear_Number";
// Player health
static constexpr auto HEALTH_ICON_FILENAME = "Heart";
static constexpr auto HEALTH_ICON_HORIZONTAL_ALIGNMENT = 0.47f;
static constexpr auto HEALTH_BAR_WIDTH = TILE_SIZE_PIXELS * 5;
static constexpr auto HEALTH_BAR_HEIGHT = TILE_SIZE_PIXELS / 3;
static constexpr auto HEALTH_BAR_HORIZONTAL_ALIGNMENT = 0.73f;
// HUD
static constexpr auto HUD_TEXT_SIZE_SMALL = 22;
static constexpr auto HUD_TEXT_SIZE_LARGE = 28;
static constexpr auto HUD_TEXT_OUTLINE_SIZE = 3.0f;
static constexpr auto HUD_VERTICAL_ALIGNMENT_TOP = 0.028f;
static constexpr auto HUD_VERTICAL_ALIGNMENT_BOTTOM = 0.96f;
static constexpr auto HUD_HORIZONTAL_ALIGNMENT_LEVEL = 0.23f;
static constexpr auto HUD_HORIZONTAL_ALIGNMENT_TIME = 0.25f;
static constexpr auto HUD_HORIZONTAL_ALIGNMENT_SCORE = 0.75f;
static constexpr auto HUD_LEVEL_TEXT = "LEVEL ";
static constexpr auto HUD_LEVEL_DIFFICULTY_SEPERATOR = " - ";
static constexpr auto HUD_TIME_TEXT = "TIME ";
static constexpr auto HUD_TIME_FORMAT_WIDTH = 2;
static constexpr auto HUD_SCORE_TEXT = "SCORE ";

static void PrintHUDText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size)
{
	PrintText(rWindow, alignment, text, size, DEFAULT_TEXT_COLOR, DEFAULT_TEXT_OUTLINE_COLOR, HUD_TEXT_OUTLINE_SIZE);
}

GameScreen::GameScreen(int& rDifficulty, int& rLevel, Timer& rTimeRemaining, Board& rSudokuBoard, Player& rPlayer, int& rKillsScore, int& rHighlightedCellIndex) :
	mr_Difficulty(rDifficulty),
	mr_Level(rLevel),
	mr_TimeRemaining(rTimeRemaining),
	mr_Board(rSudokuBoard),
	mr_Player(rPlayer),
	mr_KillsScore(rKillsScore),
	mr_HighlightedCellIndex(rHighlightedCellIndex),
	m_HealthBar(sf::Vector2f(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT), sf::Vector2f(ms_windowSize.x * HEALTH_BAR_HORIZONTAL_ALIGNMENT, ms_windowSize.y * HUD_VERTICAL_ALIGNMENT_TOP))
{
	m_Menu->AddOption(EMPTY_STR, ScreenID::PAUSE);
	SetSprite(m_HealthIcon, sf::Vector2f(ms_windowSize.x * HEALTH_ICON_HORIZONTAL_ALIGNMENT, ms_windowSize.y * HUD_VERTICAL_ALIGNMENT_TOP), HEALTH_ICON_FILENAME);
}

ScreenID GameScreen::GetID() const
{
	return ScreenID::GAME;
}

void GameScreen::HandleEventInput(sf::Event event)
{
	// Handle the player changing cell values
	if (event.type == sf::Event::KeyPressed)
	{
		// On modifiable cell ?
		if (mr_Board.IsCellHidden(mr_HighlightedCellIndex))
		{
			// Enter value
			if (IsDigitKey(event.key.code))
			{
				mr_Board.EnterCellGuess(mr_HighlightedCellIndex, GetDigitForKey(event.key.code));
				PlaySound(GAME_SCREEN_ENTER_SOUND_FILENAME);
			}
			// Clear cell, if not already empty
			else if (IsClearCellKey(event.key.code) && !mr_Board.IsCellEmpty(mr_HighlightedCellIndex))
			{
				mr_Board.ClearCellGuess(mr_HighlightedCellIndex);
				PlaySound(GAME_SCREEN_CLEAR_SOUND_FILENAME);
			}
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		// Weapon swap
		if (IsSwapWeaponKey(event.key.code))
			mr_Player.SwapWeapon();

		// Stop moving
		if (IsDirectionKey(event.key.code))
		{
			auto releasedDirection = GetDirectionForKey(event.key.code);

			// Released primary movement ?
			if (mr_Player.GetPrimaryMovement() == releasedDirection)
			{
				// Secondary becomes new primary movement (if none both become none)
				mr_Player.SetPrimaryMovement(mr_Player.GetSecondaryMovement());
				mr_Player.SetSecondaryMovement(DEFAULT_DIRECTION);
			}
			// Released secondary movement ?
			else if (mr_Player.GetSecondaryMovement() == releasedDirection)
				mr_Player.SetSecondaryMovement(DEFAULT_DIRECTION);
		}
	}
}

void GameScreen::HandleRealTimeInput()
{
	// Set shooting only when it changes
	auto shootKeyPressed = IsShootKeyPressed();
	if (mr_Player.IsUsingWeapon() != shootKeyPressed) { mr_Player.SetUsingWeapon(shootKeyPressed); }

	// Set primary movement only if player didnt have one already
	auto primMov = mr_Player.GetPrimaryMovement();
	if (primMov == Direction::NONE)
	{
		if (IsMoveLeftKeyPressed()) { mr_Player.SetPrimaryMovement(Direction::LEFT); }
		else if (IsMoveRightKeyPressed()) { mr_Player.SetPrimaryMovement(Direction::RIGHT); }
		else if (IsMoveUpKeyPressed()) { mr_Player.SetPrimaryMovement(Direction::UP); }
		else if (IsMoveDownKeyPressed()) { mr_Player.SetPrimaryMovement(Direction::DOWN); }
	}
	// Set secondary movement only if player didnt have one already
	else if (mr_Player.GetSecondaryMovement() == Direction::NONE)
	{
		// Ignore buttons pressed on the same axis as primary movement
		// so primary and secondary are not in same/opposite direction
		if (IsDirectionVertical(primMov))
		{
			if (IsMoveLeftKeyPressed()) { mr_Player.SetSecondaryMovement(Direction::LEFT); }
			else if (IsMoveRightKeyPressed()) { mr_Player.SetSecondaryMovement(Direction::RIGHT); }
		}
		else if (IsDirectionHorizontal(primMov))
		{
			if (IsMoveUpKeyPressed()) { mr_Player.SetSecondaryMovement(Direction::UP); }
			else if (IsMoveDownKeyPressed()) { mr_Player.SetSecondaryMovement(Direction::DOWN); }
		}
	}
}

void GameScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Health
	m_HealthBar.Draw(rWindow);
	rWindow.draw(m_HealthIcon);

	// Level & Difficulty
	PrintHUDText(rWindow, sf::Vector2f(HUD_HORIZONTAL_ALIGNMENT_LEVEL, HUD_VERTICAL_ALIGNMENT_TOP), m_LevelDifficultyText, HUD_TEXT_SIZE_SMALL);

	// Time
	PrintHUDText(rWindow, sf::Vector2f(HUD_HORIZONTAL_ALIGNMENT_TIME, HUD_VERTICAL_ALIGNMENT_BOTTOM), std::string(HUD_TIME_TEXT).append(m_TimeText), HUD_TEXT_SIZE_LARGE);

	// Score
	PrintHUDText(rWindow, sf::Vector2f(HUD_HORIZONTAL_ALIGNMENT_SCORE, HUD_VERTICAL_ALIGNMENT_BOTTOM), std::string(HUD_SCORE_TEXT).append(m_KillsScoreText), HUD_TEXT_SIZE_LARGE);
}

void GameScreen::Update(float dtAsSeconds)
{
	// Health
	m_HealthBar.Update(mr_Player.GetHealth(), mr_Player.GetMaxHealth());

	// Level & Difficulty
	m_LevelDifficultyText = std::string(HUD_LEVEL_TEXT).append(std::to_string(mr_Level)).append(HUD_LEVEL_DIFFICULTY_SEPERATOR).append(GetDifficultyString(mr_Difficulty));

	// Time
	auto minutes = FillWithCharacters(std::to_string(GetMinutes(mr_TimeRemaining.GetTimeLeft())), ZERO_CHAR, FillType::LEADING, HUD_TIME_FORMAT_WIDTH);
	auto seconds = FillWithCharacters(std::to_string(GetSeconds(mr_TimeRemaining.GetTimeLeft())), ZERO_CHAR, FillType::LEADING, HUD_TIME_FORMAT_WIDTH);
	m_TimeText = minutes.append(":").append(seconds);

	// Score
	m_KillsScoreText = FillWithCharacters(std::to_string(mr_KillsScore), ZERO_CHAR, FillType::LEADING, SCORE_MAX_WIDTH);
}

const std::string GameScreen::GetClassName() const
{
	return GAME_SCREEN_CLASS_NAME;
}