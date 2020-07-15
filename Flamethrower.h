#pragma once
#ifndef FLAMETHROWER_H
#define FLAMETHROWER_H

#include "Shooter.h"

static constexpr auto FLAMETHROWER_ID = 7;

class Flamethrower : public Shooter
{
public:
	Flamethrower();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const final override;

private:
	virtual void Shoot(Direction facing) final override;
	virtual sf::Vector2f GetReloadHoldOffset(Direction facing) const final override;
	virtual float GetShotDelay() const final override;
	virtual float GetReloadDelay() const final override;
	virtual bool GetLoadedAmmoVisisble() const final override;
	virtual int GetCapacity() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif