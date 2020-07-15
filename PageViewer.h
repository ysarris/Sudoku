#pragma once
#ifndef PAGE_VIEWER_H
#define PAGE_VIEWER_H

#include "Object.h"
#include "Timer.h"

static constexpr auto FIRST_PAGE = 0;

class PageViewer : public Object
{
public:
	PageViewer(sf::Vector2f pos, std::string directory, unsigned int totalPages, unsigned int defaultPageIndex = FIRST_PAGE);
	int GetCurrentPageIndex();
	void NextPage();
	void PreviousPage();
	void Update(float dtAsSeconds) final override;
	void Draw(sf::RenderWindow& rWindow) const final override;

private:
	static inline std::unique_ptr<sf::Sound> msp_NavigationSound;
	int m_TotalPages;
	int m_CurrentPage;
	Timer m_NavigationTimer;
	std::string m_PagesDirectory;

	void SwitchPage(bool playNavigationSound = true);
	virtual const std::string GetClassName() const final override;
};

#endif