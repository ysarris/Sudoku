#include "PageViewer.h"
#include "TextPrinting.h"	// for PrintText()

static constexpr auto PAGE_VIEWER_CLASS_NAME = "Page Viewer";
static constexpr auto PAGE_IMAGE_FILENAME = "Page";
static constexpr auto PAGE_NAVIGATION_SOUND_FILENAME = "Navigate";
static constexpr auto PAGE_NAVIGATION_DELAY = 0.15f;
static constexpr auto PAGE_NAVIGATION_TEXT_SIZE = 65;
static constexpr auto PAGE_NAVIGATION_OUTLINE_SIZE = 2.0f;
static constexpr auto PAGE_NAVIGATION_PREV_TEXT = "<";
static constexpr auto PAGE_NAVIGATION_NEXT_TEXT = ">";
static constexpr auto PAGE_NAVIGATION_PREV_HORIZONTAL_ALIGNMENT = 0.06f;
static constexpr auto PAGE_NAVIGATION_NEXT_HORIZONTAL_ALIGNMENT = 1.0f - PAGE_NAVIGATION_PREV_HORIZONTAL_ALIGNMENT;
static inline const auto PAGE_NAVIGATION_TEXT_COLOR = sf::Color::Black;
static inline const auto PAGE_NAVIGATION_OUTLINE_COLOR = sf::Color::White;

PageViewer::PageViewer(sf::Vector2f pos, std::string directory, unsigned int totalPages, unsigned int defaultPage) :
	Object(pos),
	m_TotalPages(totalPages),
	m_CurrentPage(defaultPage),
	m_NavigationTimer(),
	m_PagesDirectory(directory)
{
	SwitchPage(false);
}

int PageViewer::GetCurrentPageIndex()
{
	return m_CurrentPage;
}

void PageViewer::NextPage()
{
	// Must not be on last page to navigate to next
	if (m_CurrentPage != m_TotalPages - 1 && m_NavigationTimer.RanOut())
	{
		m_CurrentPage++;
		SwitchPage();
	}
}

void PageViewer::PreviousPage()
{
	// Must not be on first page to navigate to previous
	if (m_CurrentPage != FIRST_PAGE && m_NavigationTimer.RanOut())
	{
		m_CurrentPage--;
		SwitchPage();
	}
}

void PageViewer::Update(float dtAsSeconds)
{
	// Must have more than 1 page to navigate
	if (m_TotalPages > 1 && !m_NavigationTimer.RanOut()) { m_NavigationTimer.Decrement(dtAsSeconds); }
}

void PageViewer::Draw(sf::RenderWindow& rWindow) const
{
	// Draw current page
	Object::Draw(rWindow);

	// Align navigators with page's center vertically
	auto verticalAlignment = GetPos().y / rWindow.getSize().y;

	// Previous page navigation
	if (m_CurrentPage != FIRST_PAGE)
	{
		PrintText(rWindow, sf::Vector2f(PAGE_NAVIGATION_PREV_HORIZONTAL_ALIGNMENT, verticalAlignment), PAGE_NAVIGATION_PREV_TEXT,
			PAGE_NAVIGATION_TEXT_SIZE, PAGE_NAVIGATION_TEXT_COLOR, PAGE_NAVIGATION_OUTLINE_COLOR, PAGE_NAVIGATION_OUTLINE_SIZE);
	}

	// Next page navigation
	if (m_CurrentPage != m_TotalPages - 1)
	{
		PrintText(rWindow, sf::Vector2f(PAGE_NAVIGATION_NEXT_HORIZONTAL_ALIGNMENT, verticalAlignment), PAGE_NAVIGATION_NEXT_TEXT,
			PAGE_NAVIGATION_TEXT_SIZE, PAGE_NAVIGATION_TEXT_COLOR, PAGE_NAVIGATION_OUTLINE_COLOR, PAGE_NAVIGATION_OUTLINE_SIZE);
	}
}

void PageViewer::SwitchPage(bool playNavigationSound)
{
	SetSprite(PAGE_IMAGE_FILENAME + std::to_string(m_CurrentPage), m_PagesDirectory);

	if (playNavigationSound)
	{
		PlaySound(PAGE_NAVIGATION_SOUND_FILENAME, msp_NavigationSound);
		m_NavigationTimer.Reset(PAGE_NAVIGATION_DELAY);
	}
}

const std::string PageViewer::GetClassName() const
{
	return PAGE_VIEWER_CLASS_NAME;
}