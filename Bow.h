#pragma once
#ifndef BOW_H
#define BOW_H

#include "Shooter.h"

static constexpr auto BOW_ID = 5;

class Bow : public Shooter
{
public:
	Bow();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const final override;
	virtual bool IsSilent() const final override;

private:
	virtual void Shoot(Direction facing) final override;
	virtual sf::Vector2f GetReloadHoldOffset(Direction facing) const final override;
	virtual float GetHoldRotation(Direction facing) const final override;
	virtual float GetReloadDelay() const final override;
	virtual bool GetLoadedAmmoVisisble() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif