#include "PauseScreen.h"

static constexpr auto PAUSE_SCREEN_CLASS_NAME = "Pause";
static constexpr auto PAUSE_SCREEN_MENU_VERTICAL_ALIGNMENT = 0.47f;

PauseScreen::PauseScreen() :
	Screen(PAUSE_SCREEN_MENU_VERTICAL_ALIGNMENT)
{
	SetTitle(PAUSE_SCREEN_CLASS_NAME);
	m_Menu->AddOption("RESUME", ScreenID::BACK);
	m_Menu->AddOption("TUTORIAL", ScreenID::TUTORIAL);
	m_Menu->AddOption("ABANDON LEVEL", ScreenID::START);
	m_Menu->AddOption("QUIT", ScreenID::QUIT);
}

ScreenID PauseScreen::GetID() const
{
	return ScreenID::PAUSE;
}

float PauseScreen::GetBackgroundRotationRate() const
{
	return NO_ROTATION;
}

const std::string PauseScreen::GetClassName() const
{
	return PAUSE_SCREEN_CLASS_NAME;
}