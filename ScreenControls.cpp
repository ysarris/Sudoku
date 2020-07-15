#include "ScreenControls.h"

// Game
static constexpr auto QUIT_KEY = sf::Keyboard::Escape;
static constexpr auto PAUSE_KEY = sf::Keyboard::Space;
// Menu
static constexpr auto PREVIOUS_OPTION_KEY = sf::Keyboard::Up;
static constexpr auto NEXT_OPTION_KEY = sf::Keyboard::Down;
static constexpr auto SELECT_KEY = sf::Keyboard::Enter;
// Page Viewer
static constexpr auto PREVIOUS_PAGE_KEY = sf::Keyboard::Left;
static constexpr auto NEXT_PAGE_KEY = sf::Keyboard::Right;

bool WasQuitKeyPressed(sf::Event event)
{
	return (event.type == sf::Event::KeyPressed && event.key.code == QUIT_KEY) ? true : false;
}

bool WasWindowCloseAttempted(sf::Event event)
{
	return (event.type == sf::Event::Closed) ? true : false;
}

bool WasWindowFocusLost(sf::Event event)
{
	return (event.type == sf::Event::LostFocus) ? true : false;
}

bool WasPauseKeyPressed(sf::Event event)
{
	return (event.type == sf::Event::KeyReleased && event.key.code == PAUSE_KEY) ? true : false;
}

bool IsPrevOptionKeyPressed()
{
	return sf::Keyboard::isKeyPressed(PREVIOUS_OPTION_KEY);
}

bool IsNextOptionKeyPressed()
{
	return sf::Keyboard::isKeyPressed(NEXT_OPTION_KEY);
}

bool WasSelectKeyPressed(sf::Event event)
{
	return (event.type == sf::Event::KeyReleased && event.key.code == SELECT_KEY) ? true : false;
}

bool IsPrevPageKeyPressed()
{
	return sf::Keyboard::isKeyPressed(PREVIOUS_PAGE_KEY);
}

bool IsNextPageKeyPressed()
{
	return sf::Keyboard::isKeyPressed(NEXT_PAGE_KEY);
}