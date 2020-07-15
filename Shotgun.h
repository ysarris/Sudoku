#pragma once
#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "Shooter.h"

static constexpr auto SHOTGUN_ID = 3;

class Shotgun : public Shooter
{
public:
	Shotgun();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const final override;

private:
	virtual void Shoot(Direction facing) final override;
	virtual float GetShotDelay() const final override;
	virtual float GetReloadDelay() const final override;
	virtual int GetCapacity() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif