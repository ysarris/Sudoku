#pragma once
#ifndef MENU_H
#define MENU_H

#include "Object.h"
#include "Timer.h"
#include "SizeStatus.h"
enum class ScreenID;	// forward declaration

static constexpr auto MENU_DEFAULT_SELECTED_INDEX = 0;

struct MenuOption
{
	std::string m_Text;
	ScreenID m_DestinationScreenID;

	MenuOption(std::string text, ScreenID ID) : m_Text(text), m_DestinationScreenID(ID) {}
};

class Menu : public Object
{
public:
	Menu(sf::Vector2f pos, int defaultSelectedIndex = MENU_DEFAULT_SELECTED_INDEX);
	bool IsEmpty() const;
	void AddOption(std::string buttonText, ScreenID destinationScreenID);
	void NextOption();
	void PreviousOption();
	void ClearAllOptions();
	ScreenID GetSelectedDestination() const;
	void Update(float dtAsSeconds) final override;
	void Draw(sf::RenderWindow& rWindow) const final override;

private:
	static inline std::unique_ptr<sf::Sound> msp_NavigationSound;
	std::vector<MenuOption> m_Options;
	int m_SelectedIndex;
	float m_SelectedTextSize;
	SizeStatus m_SelectedTextStatus;
	Timer m_NavigationTimer;

	void SwitchOption();
	void UpdateSelectedTextSize(float dtAsSeconds);
	virtual const std::string GetClassName() const final override;
};

#endif