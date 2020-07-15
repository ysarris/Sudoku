#include "Menu.h"
#include "TextPrinting.h"	// for PrintText()

static constexpr auto MENU_CLASS_NAME = "Menu";
static constexpr auto MENU_NAVIGATION_SOUND_FILENAME = "Navigate";
static constexpr auto MENU_NAVIGATION_DELAY = 0.15f;
static constexpr auto MENU_TEXT_SIZE_DEFAULT = 35;
static constexpr auto MENU_TEXT_SIZE_SELECTED_MIN = 45.0f;
static constexpr auto MENU_TEXT_SIZE_SELECTED_MAX = 55.0f;
static constexpr auto MENU_TEXT_SIZE_CHANGE_RATE = 33.0f;
static constexpr auto MENU_TEXT_OUTLINE_SIZE_DEFAULT = 0.0f;
static constexpr auto MENU_TEXT_OUTLINE_SIZE_SELECTED = 3.0f;
static constexpr auto MENU_TEXT_STYLE_SELECTED = TextStyle::BOLD;
static constexpr auto MENU_OPTION_SPACING = 50.0f;
static inline const auto MENU_TEXT_COLOR_SELECTED = sf::Color::Black;
static inline const auto MENU_TEXT_OUTLINE_COLOR = sf::Color::White;

Menu::Menu(sf::Vector2f pos, int defaultSelectedIndex) :
	Object(pos),
	m_SelectedIndex(defaultSelectedIndex),
	m_SelectedTextSize(MENU_TEXT_SIZE_SELECTED_MIN),
	m_SelectedTextStatus(DEFAULT_SIZE_STATUS),
	m_NavigationTimer()
{
}

void Menu::AddOption(std::string buttonText, ScreenID destinationScreenID)
{
	m_Options.emplace_back(buttonText, destinationScreenID);
}

ScreenID Menu::GetSelectedDestination() const
{
	return m_Options.at(m_SelectedIndex).m_DestinationScreenID;
}

bool Menu::IsEmpty() const
{
	return static_cast<int>(m_Options.size()) == 0;
}

void Menu::NextOption()
{
	// Must have more than 1 option to navigate
	if (static_cast<int>(m_Options.size()) > 1 && m_NavigationTimer.RanOut())
	{
		m_SelectedIndex = ++m_SelectedIndex % static_cast<int>(m_Options.size());
		SwitchOption();
	}
}

void Menu::PreviousOption()
{
	// Must have more than 1 option to navigate
	if (static_cast<int>(m_Options.size()) > 1 && m_NavigationTimer.RanOut())
	{
		m_SelectedIndex = (--m_SelectedIndex < 0) ? (static_cast<int>(m_Options.size()) - 1) : m_SelectedIndex;
		SwitchOption();
	}
}

void Menu::ClearAllOptions()
{
	m_Options.clear();
}

void Menu::Update(float dtAsSeconds)
{
	// Must have more than 1 option to navigate
	if (static_cast<int>(m_Options.size()) > 1 && !m_NavigationTimer.RanOut()) { m_NavigationTimer.Decrement(dtAsSeconds); }
	UpdateSelectedTextSize(dtAsSeconds);
}

void Menu::Draw(sf::RenderWindow& rWindow) const
{
	for (auto menu_it = m_Options.begin(); menu_it != m_Options.end(); menu_it++)
	{
		// Use default text configuration
		auto size = MENU_TEXT_SIZE_DEFAULT;
		auto textStyle = DEFAULT_TEXT_STYLE;
		auto outlineThickness = MENU_TEXT_OUTLINE_SIZE_DEFAULT;
		auto color = DEFAULT_TEXT_COLOR;

		// Current option is selected ?
		auto menuOptionIndex = std::distance(m_Options.begin(), menu_it);
		if (menuOptionIndex == m_SelectedIndex)
		{
			// Use selected text configuration
			size = static_cast<int>(m_SelectedTextSize);
			textStyle = MENU_TEXT_STYLE_SELECTED;
			outlineThickness = MENU_TEXT_OUTLINE_SIZE_SELECTED;
			color = MENU_TEXT_COLOR_SELECTED;
		}

		// Option details
		auto verticalAlignment = (GetPos().y + menuOptionIndex * MENU_OPTION_SPACING) / rWindow.getSize().y;
		auto text = (*menu_it).m_Text;

		PrintText(rWindow, sf::Vector2f(CENTER_ALIGNMENT, verticalAlignment), text,
			size, color, MENU_TEXT_OUTLINE_COLOR, outlineThickness, DEFAULT_FONT_STYLE, textStyle);
	}
}

void Menu::SwitchOption()
{
	PlaySound(MENU_NAVIGATION_SOUND_FILENAME, msp_NavigationSound);
	m_NavigationTimer.Reset(MENU_NAVIGATION_DELAY);
}

void Menu::UpdateSelectedTextSize(float dtAsSeconds)
{
	// Change text size according to status
	auto sizeChange = MENU_TEXT_SIZE_CHANGE_RATE * dtAsSeconds;
	m_SelectedTextSize += (m_SelectedTextStatus == SizeStatus::GROWING) ? sizeChange : -sizeChange;

	// Stay within limits
	if (m_SelectedTextSize > MENU_TEXT_SIZE_SELECTED_MAX)
	{
		m_SelectedTextSize = MENU_TEXT_SIZE_SELECTED_MAX;
		m_SelectedTextStatus = SizeStatus::SHRINKING;
	}
	else if (m_SelectedTextSize < MENU_TEXT_SIZE_SELECTED_MIN)
	{
		m_SelectedTextSize = MENU_TEXT_SIZE_SELECTED_MIN;
		m_SelectedTextStatus = SizeStatus::GROWING;
	}
}

const std::string Menu::GetClassName() const
{
	return MENU_CLASS_NAME;
}