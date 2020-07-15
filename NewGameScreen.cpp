#include "NewGameScreen.h"
#include "ScreenControls.h"		// for Page Viewer input functions
#include "Difficulty_info.h"	// for GetNumberOfDifficulties() and GetDefaultDifficulty()

static constexpr auto NEWGAME_SCREEN_CLASS_NAME = "New Game";
static constexpr auto NEWGAME_SCREEN_TITLE = "Choose difficulty";
static constexpr auto NEWGAME_SCREEN_MENU_VERTICAL_ALIGNMENT = 0.88f;

NewGameScreen::NewGameScreen() :
	Screen(NEWGAME_SCREEN_MENU_VERTICAL_ALIGNMENT),
	m_DifficultyViewer(std::make_unique<PageViewer>(ms_windowSize* CENTER_ALIGNMENT, GetDirectory(), GetNumberOfDifficulties(), GetDefaultDifficulty()))
{
	SetTitle(NEWGAME_SCREEN_TITLE);
	m_Menu->AddOption("BACK", ScreenID::BACK);
	m_Menu->AddOption("PLAY", ScreenID::GAME);
}

int NewGameScreen::GetDifficulty() const
{
	return m_DifficultyViewer->GetCurrentPageIndex();
}

ScreenID NewGameScreen::GetID() const
{
	return ScreenID::NEW_GAME;
}

void NewGameScreen::HandleRealTimeInput()
{
	// Default handling
	Screen::HandleRealTimeInput();

	// Navigating pages
	if (IsPrevPageKeyPressed()) { m_DifficultyViewer->PreviousPage(); }
	else if (IsNextPageKeyPressed()) { m_DifficultyViewer->NextPage(); }
}

void NewGameScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Default screen draw
	Screen::Draw(rWindow);

	m_DifficultyViewer->Draw(rWindow);
}

void NewGameScreen::Update(float dtAsSeconds)
{
	// Default screen updates
	Screen::Update(dtAsSeconds);

	m_DifficultyViewer->Update(dtAsSeconds);
}

const std::string NewGameScreen::GetClassName() const
{
	return NEWGAME_SCREEN_CLASS_NAME;
}