#include "QuitScreen.h"

static constexpr auto QUIT_SCREEN_CLASS_NAME = "Quit";
static constexpr auto QUIT_SCREEN_TITLE = "Quit game ?";
static constexpr auto QUIT_SCREEN_TITLE_VERTICAL_ALIGNMENT = 0.4f;
static constexpr auto QUIT_SCREEN_MENU_VERTICAL_ALIGNMENT = 0.55f;

QuitScreen::QuitScreen() :
	Screen(QUIT_SCREEN_MENU_VERTICAL_ALIGNMENT, QUIT_SCREEN_TITLE_VERTICAL_ALIGNMENT)
{
	SetTitle(QUIT_SCREEN_TITLE);
	SetSprite(QUIT_SCREEN_CLASS_NAME);
	m_Menu->AddOption("NO", ScreenID::BACK);
	m_Menu->AddOption("YES", ScreenID::CLOSED);
}

ScreenID QuitScreen::GetID() const
{
	return ScreenID::QUIT;
}

float QuitScreen::GetBackgroundRotationRate() const
{
	// Use default rotation but anticlockwise
	return -Screen::GetBackgroundRotationRate();
}

const std::string QuitScreen::GetClassName() const
{
	return QUIT_SCREEN_CLASS_NAME;
}