#include "HighscoresScreen.h"
#include "StringManipulation.h"		// for FillWithCharacters()
#include "Highscore_info.h"			// for NAME_MAX_WIDTH, SCORE_MAX_WIDTH and MAX_HIGHSCORES_STORED

static constexpr auto HIGHSCORES_SCREEN_CLASS_NAME = "Highscores";
static constexpr auto HIGHSCORES_VERTICAL_ALIGNMENT = 0.19f;
static constexpr auto HIGHSCORES_TEXT_SIZE = 30;
static constexpr auto HIGHSCORES_FONT_STYLE = FontStyle::SYMMETRIC;
static constexpr auto HIGHSCORES_LINE_SPACING = 20.0f;

static std::string ExtractHighscores(const std::vector<std::pair<std::string, int>>& rHighscores)
{
	auto highscoreMaxLength = NAME_MAX_WIDTH + SCORE_MAX_WIDTH + 1; // +1 to separate name and score
	auto storedHighscores = static_cast<int>(rHighscores.size());
	std::string curHighscore, allHighscores;

	for (auto i = 0; i < MAX_HIGHSCORES_STORED; i++)
	{
		// Add rank
		curHighscore = FillWithCharacters(std::to_string(i + 1) + DOT, SPACE_CHAR, FillType::LEADING, RANK_MAX_WIDTH + 1); // +1 to separate rank and name

		// Valid entry, add name and score
		if (i < storedHighscores)
			curHighscore.append(FillWithCharactersInBetween(rHighscores.at(i).first, std::to_string(rHighscores.at(i).second), SPACE_CHAR, highscoreMaxLength));
		// Empty entry
		else
			curHighscore.append(FillWithCharacters(EMPTY_STR, DASH, FillType::TRAILING, highscoreMaxLength));

		// Add current highscore to text
		allHighscores.append(curHighscore).append(NEWLINE);
	}
	return allHighscores;
}

HighscoresScreen::HighscoresScreen(const std::vector <std::pair<std::string, int>>& rHighscores, bool newScoreJustEntered)
{
	SetTitle(HIGHSCORES_SCREEN_CLASS_NAME);
	SetSprite(HIGHSCORES_SCREEN_CLASS_NAME);
	m_HighscoresText = ExtractHighscores(rHighscores);
	(newScoreJustEntered) ? m_Menu->AddOption("DONE", ScreenID::START) : m_Menu->AddOption("BACK", ScreenID::BACK);
}

ScreenID HighscoresScreen::GetID() const
{
	return ScreenID::HIGHSCORES;
}

void HighscoresScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Default screen draw
	Screen::Draw(rWindow);

	PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, HIGHSCORES_VERTICAL_ALIGNMENT), m_HighscoresText,
		HIGHSCORES_TEXT_SIZE, DEFAULT_TEXT_COLOR, HIGHSCORES_FONT_STYLE, DEFAULT_TEXT_STYLE, HIGHSCORES_LINE_SPACING);
}

const std::string HighscoresScreen::GetClassName() const
{
	return HIGHSCORES_SCREEN_CLASS_NAME;
}