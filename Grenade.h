#pragma once
#ifndef GRENADE_H
#define GRENADE_H

#include "Throwable.h"
#include "Timer.h"

static constexpr auto GRENADE_ID = 4;

class Grenade : public Throwable
{
public:
	// Weapon
	Grenade();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const override;

	// Projectile
	Grenade(sf::Vector2f startPos, Direction facing, float floorCoordinate, float startHeight);
	virtual void HitWall(sf::Vector2f hitPos) final override;
	virtual void HitFloor() final override;
	virtual void HitActor(Actor& actor) final override;

	// Both
	virtual void Update(float dtAsSeconds) final override;

private:
	// Weapon
	virtual void Throw(Direction facing, float floorCoordinate, float startHeight) final override;
	virtual float GetReloadDelay() const final override;

	// Projectile
	Timer m_DetonationTimer;
	virtual bool GetIsExplosive() const final override;
	virtual int GetDamage() const final override;
	virtual float GetMaxRange() const final override;
	virtual float GetRotationRate() const final override;

	// Both
	virtual const std::string GetClassName() const final override;
};

#endif