#include "Screen.h"
#include "TextPrinting.h"			// for PrintText()
#include "StringManipulation.h"		// for ContainsNewLine()
#include "ScreenControls.h"			// for Menu input functions

static constexpr auto SCREEN_CLASS_NAME = "Screen";
static constexpr auto SCREEN_DEFAULT_BACKGROUND_IMAGE_FILENAME = "Default_Background";
static constexpr auto SCREEN_DEFAULT_BACKGROUND_ROTATION_RATE = 10.0f;
static constexpr auto TITLE_MAX_LENGTH = 18;
static constexpr auto TITLE_TEXT_SIZE = 50;
static constexpr auto TITLE_TEXT_OUTLINE_SIZE = 4.0f;
static constexpr auto ERROR_WINDOW_SIZE = "Invalid or no window size set - call SetWindowSize() with dimensions greater than 0.";
static constexpr auto ERROR_SELECT_EMPTY_MENU = "Trying to select option in empty menu - must have at least 1 option.";
static constexpr auto ERROR_TITLE_TOO_BIG = "Title length is too big - must be no more than 18 characters per line.";
static constexpr auto ERROR_TITLE_CONTAINS_NEWLINE = "Title cannot contain a newline character - use two lined title instead.";

void Screen::SetWindowSize(sf::Vector2f windowSize)
{
	ms_windowSize = windowSize;
}

bool Screen::IsReadyToSelect() const
{
	return !m_Menu->IsEmpty();
}

ScreenID Screen::Select() const
{
	Assert(ERROR_SELECT_EMPTY_MENU, IsReadyToSelect());
	return m_Menu->GetSelectedDestination();
}

void Screen::HandleEventInput(sf::Event event)
{
	// None for default screen
}

void Screen::HandleRealTimeInput()
{
	// Navigating menu
	if (IsPrevOptionKeyPressed()) { m_Menu->PreviousOption(); }
	else if (IsNextOptionKeyPressed()) { m_Menu->NextOption(); }
}

void Screen::Update(float dtAsSeconds)
{
	// Rotate background
	RotateSpriteBy(dtAsSeconds * GetBackgroundRotationRate());
	m_Menu->Update(dtAsSeconds);
}

void Screen::Draw(sf::RenderWindow& rWindow) const
{
	// Draw background
	Object::Draw(rWindow);

	// Dont print empty title
	if (static_cast<int>(m_Title.size()) != 0)
		PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, m_TitleVerticalAlignment), m_Title, TITLE_TEXT_SIZE,
			DEFAULT_TEXT_COLOR, DEFAULT_TEXT_OUTLINE_COLOR, TITLE_TEXT_OUTLINE_SIZE);

	m_Menu->Draw(rWindow);
}

Screen::Screen(float menuVerticalAlignment, float titleVerticalAlignment) :
	Object(ms_windowSize * CENTER_ALIGNMENT),
	m_Menu(std::make_unique<Menu>(sf::Vector2f(ms_windowSize.x * CENTER_ALIGNMENT, ms_windowSize.y * menuVerticalAlignment))),
	m_TitleVerticalAlignment(titleVerticalAlignment)
{
	Assert(ERROR_WINDOW_SIZE, ms_windowSize.x > 0 || ms_windowSize.y > 0);
	// Set default background, derived screens can overwrite it
	SetSprite(SCREEN_DEFAULT_BACKGROUND_IMAGE_FILENAME);
	AddToDirectory(SCREEN_CLASS_NAME); // Add after default background is set
}

void Screen::SetTitle(std::string text)
{
	Assert(ERROR_TITLE_TOO_BIG, text.length() <= TITLE_MAX_LENGTH);
	Assert(ERROR_TITLE_CONTAINS_NEWLINE, !ContainsNewLine(text).first);
	m_Title = text;
}

void Screen::SetTitle(std::string textLine1, std::string textLine2)
{
	Assert(ERROR_TITLE_TOO_BIG, textLine1.length() <= TITLE_MAX_LENGTH);
	Assert(ERROR_TITLE_TOO_BIG, textLine2.length() <= TITLE_MAX_LENGTH);
	Assert(ERROR_TITLE_CONTAINS_NEWLINE, !ContainsNewLine(textLine1).first);
	Assert(ERROR_TITLE_CONTAINS_NEWLINE, !ContainsNewLine(textLine2).first);
	m_Title = textLine1.append(NEWLINE).append(textLine2);
}

float Screen::GetBackgroundRotationRate() const
{
	return SCREEN_DEFAULT_BACKGROUND_ROTATION_RATE;
}

const std::string Screen::GetClassName() const
{
	return SCREEN_CLASS_NAME;
}