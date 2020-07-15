#pragma once
#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "Screen.h"

class StartScreen : public Screen
{
public:
	StartScreen(bool gamesaveExists);
	virtual ScreenID GetID() const final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;

private:
	sf::Sprite m_Logo;

	virtual const std::string GetClassName() const final override;
};

#endif