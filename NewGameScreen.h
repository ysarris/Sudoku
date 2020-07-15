#pragma once
#ifndef NEWGAME_SCREEN_H
#define NEWGAME_SCREEN_H

#include "Screen.h"
#include "PageViewer.h"

class NewGameScreen : public Screen
{
public:
	NewGameScreen();
	int GetDifficulty() const;
	virtual ScreenID GetID() const final override;
	virtual void HandleRealTimeInput() final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	std::unique_ptr<PageViewer> m_DifficultyViewer;

	virtual const std::string GetClassName() const final override;
};

#endif