#pragma once
#ifndef HIGHSCORE_SCREEN_H
#define HIGHSCORE_SCREEN_H

#include "Screen.h"

class HighscoresScreen : public Screen
{
public:
	HighscoresScreen(const std::vector<std::pair<std::string, int>>& rHighscores, bool newScoreJustEntered);
	virtual ScreenID GetID() const final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;

private:
	std::string m_HighscoresText;

	virtual const std::string GetClassName() const final override;
};

#endif