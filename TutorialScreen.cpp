#include "TutorialScreen.h"
#include "ScreenControls.h"		// for Page Viewer input functions

static constexpr auto TUTORIAL_SCREEN_CLASS_NAME = "Tutorial";
static constexpr auto TUTORIAL_TOTAL_PAGES = 6;

TutorialScreen::TutorialScreen() :
	m_TutorialViewer(std::make_unique<PageViewer>(ms_windowSize * CENTER_ALIGNMENT, GetDirectory(), TUTORIAL_TOTAL_PAGES))
{
	SetTitle(TUTORIAL_SCREEN_CLASS_NAME);
	SetSprite(TUTORIAL_SCREEN_CLASS_NAME);
	m_Menu->AddOption("BACK", ScreenID::BACK);
}

ScreenID TutorialScreen::GetID() const
{
	return ScreenID::TUTORIAL;
}

void TutorialScreen::HandleRealTimeInput()
{
	// Default handling
	Screen::HandleRealTimeInput();

	// Navigating pages
	if (IsPrevPageKeyPressed()) { m_TutorialViewer->PreviousPage(); }
	else if (IsNextPageKeyPressed()) { m_TutorialViewer->NextPage(); }
}

void TutorialScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Default screen draw
	Screen::Draw(rWindow);

	m_TutorialViewer->Draw(rWindow);
}

void TutorialScreen::Update(float dtAsSeconds)
{
	// Default screen updates
	Screen::Update(dtAsSeconds);

	m_TutorialViewer->Update(dtAsSeconds);
}

const std::string TutorialScreen::GetClassName() const
{
	return TUTORIAL_SCREEN_CLASS_NAME;
}