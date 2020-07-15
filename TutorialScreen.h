#pragma once
#ifndef TUTORIAL_SCREEN_H
#define TUTORIAL_SCREEN_H

#include "Screen.h"
#include "PageViewer.h"

class TutorialScreen : public Screen
{
public:
	TutorialScreen();
	virtual ScreenID GetID() const final override;
	virtual void HandleRealTimeInput() final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	std::unique_ptr<PageViewer> m_TutorialViewer;

	virtual const std::string GetClassName() const final override;
};

#endif