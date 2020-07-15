#pragma once
#ifndef PISTOL_H
#define PISTOL_H

#include "Shooter.h"

static constexpr auto PISTOL_ID = 1;

class Pistol : public Shooter
{
public:
	Pistol();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const final override;

private:
	virtual void Shoot(Direction facing) final override;
	virtual float GetShotDelay() const final override;
	virtual float GetReloadDelay() const final override;
	virtual int GetCapacity() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif