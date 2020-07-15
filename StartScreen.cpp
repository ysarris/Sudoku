#include "StartScreen.h"

static constexpr auto START_SCREEN_CLASS_NAME = "Start";
static constexpr auto START_SCREEN_LOGO_FILENAME = "Logo";
static constexpr auto START_SCREEN_LOGO_VERTICAL_ALIGNMENT = 0.28f;
static constexpr auto START_SCREEN_MENU_VERTICAL_ALIGNMENT = 0.6f;

StartScreen::StartScreen(bool gamesaveExists) :
	Screen(START_SCREEN_MENU_VERTICAL_ALIGNMENT)
{
	SetSprite(m_Logo, sf::Vector2f(ms_windowSize.x * CENTER_ALIGNMENT, ms_windowSize.y * START_SCREEN_LOGO_VERTICAL_ALIGNMENT), START_SCREEN_LOGO_FILENAME);
	// Add continue option if save file exists
	if (gamesaveExists) { m_Menu->AddOption("Continue", ScreenID::GAME); }
	m_Menu->AddOption("New Game", ScreenID::NEW_GAME);
	m_Menu->AddOption("Highscores", ScreenID::HIGHSCORES);
	m_Menu->AddOption("Tutorial", ScreenID::TUTORIAL);
	m_Menu->AddOption("Quit", ScreenID::QUIT);
}

ScreenID StartScreen::GetID() const
{
	return ScreenID::START;
}

void StartScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Default screen draw
	Screen::Draw(rWindow);
	
	rWindow.draw(m_Logo);
}

const std::string StartScreen::GetClassName() const
{
	return START_SCREEN_CLASS_NAME;
}