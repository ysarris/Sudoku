#include "NameEntryScreen.h"
#include "Highscore_info.h"		// for NAME_MAX_WIDTH

static constexpr auto NES_CLASS_NAME = "Name Entry";
static constexpr auto NES_TITLE = "Enter your name";
static constexpr auto NES_INPUT_DELAY = 0.04f;
static constexpr auto NES_READ_FILENAME = "read";
static constexpr auto NES_DELETE_FILENAME = "delete";
static constexpr auto NES_WARNING_FILENAME = "warning";
static constexpr auto NES_NAME_TEXT_SIZE = 50;
static constexpr auto NES_NAME_TEXT_STYLE = TextStyle::BOLD;
static constexpr auto NES_NAME_FONT_STYLE = FontStyle::SYMMETRIC;
static constexpr auto NES_WARNING_TEXT_SIZE = 20;
static constexpr auto NES_WARNING_TEXT_VERTICAL_ALIGNMENT = 0.6f;
static constexpr auto NES_WARNING_TEXT_MIN = "1 character at least";
static inline const auto NES_WARNING_TEXT_MAX = std::to_string(NAME_MAX_WIDTH).append(" characters max");
static inline const auto NES_WARNING_TEXT_COLOR = sf::Color::Yellow;
// Keys
static constexpr auto CODE_CAPITAL_A = 65;
static constexpr auto CODE_CAPITAL_Z = 90;
static constexpr auto CODE_SMALL_A = 97;
static constexpr auto CODE_SMALL_Z = 122;
static constexpr auto CODE_CARRIAGE_RETURN = 13;
static constexpr auto DELETE_CHAR_KEY = sf::Keyboard::BackSpace;

// Requires code to be decimal unicode
static bool IsAlphabetLetter(int code)
{
	return ((code >= CODE_CAPITAL_A && code <= CODE_CAPITAL_Z)
			|| (code >= CODE_SMALL_A && code <= CODE_SMALL_Z)) ? true : false;
}

NameEntryScreen::NameEntryScreen() :
	m_Name(),
	m_InputTimer()
{
	SetTitle(NES_TITLE);
}

std::string NameEntryScreen::GetNameEntered() const
{
	return m_Name;
}

ScreenID NameEntryScreen::GetID() const
{
	return ScreenID::ENTER_HIGHSCORE_NAME;
}

void NameEntryScreen::HandleEventInput(sf::Event event)
{
	if (m_InputTimer.RanOut())
	{
		// Read character
		if (event.type == sf::Event::TextEntered)
		{
			m_InputTimer.Reset(NES_INPUT_DELAY);

			// Only accept alphabetic characters and stay within max name length
			if (IsAlphabetLetter(event.text.unicode) && static_cast<int>(m_Name.size()) < NAME_MAX_WIDTH)
			{
				m_Name += static_cast<char>(event.text.unicode);
				PlaySound(NES_READ_FILENAME);
			}
			// Play error sound unless character is the carriage return and name is not empty
			else if (event.text.unicode != CODE_CARRIAGE_RETURN || static_cast<int>(m_Name.size()) == 0)
				PlaySound(NES_WARNING_FILENAME);
		}

		// Delete character
		if (event.type == sf::Event::KeyPressed && event.key.code == DELETE_CHAR_KEY)
		{
			m_InputTimer.Reset(NES_INPUT_DELAY);

			if (static_cast<int>(m_Name.size()) > 0)
			{
				m_Name.pop_back();
				PlaySound(NES_DELETE_FILENAME);
			}
			else
				PlaySound(NES_WARNING_FILENAME);
		}
	}
}

void NameEntryScreen::Draw(sf::RenderWindow& rWindow) const
{
	// Default screen draw
	Screen::Draw(rWindow);
	
	// Any letters entered ?
	if (static_cast<int>(m_Name.size()) > 0)
	{
		PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, CENTER_ALIGNMENT),
			m_Name, NES_NAME_TEXT_SIZE, DEFAULT_TEXT_COLOR, NES_NAME_FONT_STYLE, NES_NAME_TEXT_STYLE);

		// Max warning
		if (static_cast<int>(m_Name.size()) == NAME_MAX_WIDTH)
			PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, NES_WARNING_TEXT_VERTICAL_ALIGNMENT),
				NES_WARNING_TEXT_MAX, NES_WARNING_TEXT_SIZE, NES_WARNING_TEXT_COLOR);
	}
	else
	{
		// Min warning
		PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, NES_WARNING_TEXT_VERTICAL_ALIGNMENT),
			NES_WARNING_TEXT_MIN, NES_WARNING_TEXT_SIZE, NES_WARNING_TEXT_COLOR);
	}
}

void NameEntryScreen::Update(float dtAsSeconds)
{
	// Default screen updates
	Screen::Update(dtAsSeconds);

	// Name entry screen updates
	if (!m_InputTimer.RanOut()) { m_InputTimer.Decrement(dtAsSeconds); }
	// Clear menu if name just became empty
	if (static_cast<int>(m_Name.size()) == 0 && !m_Menu->IsEmpty()) { m_Menu->ClearAllOptions(); }
	// Add menu option if name just stopped being empty
	else if (static_cast<int>(m_Name.size()) > 0 && m_Menu->IsEmpty()) { m_Menu->AddOption("DONE", ScreenID::HIGHSCORES); }
}

const std::string NameEntryScreen::GetClassName() const
{
	return NES_CLASS_NAME;
}