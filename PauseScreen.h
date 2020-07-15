#pragma once
#ifndef PAUSE_SCREEN_H
#define PAUSE_SCREEN_H

#include "Screen.h"

class PauseScreen : public Screen
{
public:
	PauseScreen();
	virtual ScreenID GetID() const final override;

private:
	virtual float GetBackgroundRotationRate() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif