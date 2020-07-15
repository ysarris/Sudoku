#pragma once
#ifndef NAME_ENTRY_SCREEN_H
#define NAME_ENTRY_SCREEN_H

#include "Screen.h"
#include "Timer.h"

class NameEntryScreen : public Screen
{
public:
	NameEntryScreen();
	std::string GetNameEntered() const;
	virtual ScreenID GetID() const final override;
	virtual void HandleEventInput(sf::Event event) final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	Timer m_InputTimer;
	std::string m_Name;

	virtual const std::string GetClassName() const final override;
};

#endif