#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include "Object.h"
#include "Menu.h"
#include "Screens_info.h"	// for screen IDs
#include "TextPrinting.h"	// for PrintText()

static constexpr auto MENU_DEFAULT_VERTICAL_ALIGNMENT = 0.95f;
static constexpr auto TITLE_DEFAULT_VERTICAL_ALIGNMENT = 0.075f;

class Screen : public Object
{
public:
	static void SetWindowSize(sf::Vector2f windowSize);
	bool IsReadyToSelect() const;
	ScreenID Select() const;
	virtual ~Screen() noexcept = default;
	virtual ScreenID GetID() const = 0;
	virtual void HandleEventInput(sf::Event event);
	virtual void HandleRealTimeInput();
	virtual void Update(float dtAsSeconds) override;
	virtual void Draw(sf::RenderWindow& rWindow) const override;

protected:
	static inline sf::Vector2f ms_windowSize;
	std::unique_ptr<Menu> m_Menu;

	Screen(float menuVerticalAlignment = MENU_DEFAULT_VERTICAL_ALIGNMENT, float titleVerticalAlignment = TITLE_DEFAULT_VERTICAL_ALIGNMENT);
	void SetTitle(std::string text);
	void SetTitle(std::string textLine1, std::string textLine2);
	virtual float GetBackgroundRotationRate() const;

private:
	std::string m_Title;
	float m_TitleVerticalAlignment;

	virtual const std::string GetClassName() const override;
};

#endif