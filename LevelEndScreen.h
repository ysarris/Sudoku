#pragma once
#ifndef LEVEL_END_SCREEN_H
#define LEVEL_END_SCREEN_H

#include "Screen.h"

class LevelEndScreen : public Screen
{
public:
	LevelEndScreen(int level); // Failed
	LevelEndScreen(int level, bool isNewHighscore, int killPoints, int solvedPoints, int timePoints, int totalPoints); // Completed
	virtual ScreenID GetID() const final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;

private:
	std::string m_ScoreBreakdown;
	std::string m_GameCompleteMessage;
	std::string m_NewWeaponMessage;

	virtual const std::string GetClassName() const final override;
};

#endif