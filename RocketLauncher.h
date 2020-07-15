#pragma once
#ifndef ROCKETLAUNCHER_H
#define ROCKETLAUNCHER_H

#include "Shooter.h"

static constexpr auto ROCKETLAUNCHER_ID = 6;

class RocketLauncher : public Shooter
{
public:
	RocketLauncher();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const final override;

private:
	virtual void Shoot(Direction facing) final override;
	virtual sf::Vector2f GetReloadHoldOffset(Direction facing) const final override;
	virtual float GetReloadDelay() const final override;
	virtual bool GetLoadedAmmoVisisble() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif