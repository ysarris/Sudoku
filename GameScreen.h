#pragma once
#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "Screen.h"
#include "Timer.h"
#include "Board.h"
#include "Player.h"
#include "HealthBar.h"

class GameScreen : public Screen
{
public:
	GameScreen(int& difficulty, int& level, Timer& rTimeRemaining, Board& rSudokuBoard, Player& rPlayer, int& rKillsScore, int& rHighlightedCellIndex);
	virtual ScreenID GetID() const final override;
	virtual void HandleEventInput(sf::Event event) final override;
	virtual void HandleRealTimeInput() final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	int& msp_Difficulty;
	int& mr_Level;
	Timer& mr_TimeRemaining;
	Board& mr_Board;
	Player& mr_Player;
	int& mr_KillsScore;
	int& mr_HighlightedCellIndex;
	sf::Sprite m_HealthIcon;
	HealthBar m_HealthBar;
	std::string m_KillsScoreText, m_TimeText, m_LevelDifficultyText;

	virtual const std::string GetClassName() const final override;
};

#endif