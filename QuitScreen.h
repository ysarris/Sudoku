#pragma once
#ifndef QUIT_SCREEN_H
#define QUIT_SCREEN_H

#include "Screen.h"

class QuitScreen : public Screen
{
public:
	QuitScreen();
	virtual ScreenID GetID() const final override;

private:
	virtual float GetBackgroundRotationRate() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif