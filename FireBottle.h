#pragma once
#ifndef FIRE_BOTTLE_H
#define FIRE_BOTTLE_H

#include "Throwable.h"

static constexpr auto FIRE_BOTTLE_ID = 8;

class FireBottle : public Throwable
{
public:
	// Weapon
	FireBottle();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const override;

	// Projectile
	FireBottle(sf::Vector2f startPos, Direction facing, float floorCoordinate, float startHeight);
	virtual void HitWall(sf::Vector2f hitPos) final override;
	virtual void HitFloor() final override;
	virtual void HitActor(Actor& actor) final override;

private:
	// Weapon
	virtual void Throw(Direction facing, float floorCoordinate, float startHeight) final override;
	virtual float GetHoldRotation(Direction facing) const final override;
	virtual float GetReloadDelay() const final override;

	// Projectile
	void Smash();
	virtual int GetDamage() const final override;
	virtual float GetMaxRange() const final override;
	virtual float GetRotationRate() const final override;

	// Both
	virtual const std::string GetClassName() const final override;
};

#endif