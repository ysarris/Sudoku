#pragma once
#ifndef HATCHET_H
#define HATCHET_H

#include "Throwable.h"

static constexpr auto HATCHET_ID = 2;

class Hatchet : public Throwable
{
public:
	// Weapon
	Hatchet();
	virtual sf::Vector2f GetHoldOffset(Direction facing) const final override;

	// Projectile
	Hatchet(sf::Vector2f startPos, Direction facing, float floorCoordinate, float startHeight);
	virtual void HitFloor() final override;
	virtual void HitWall(sf::Vector2f hitPos) final override;
	virtual void HitActor(Actor& actor) final override;

private:
	// Weapon
	virtual void Throw(Direction facing, float floorCoordinate, float startHeight) final override;
	virtual float GetHoldRotation(Direction facing) const final override;
	virtual float GetReloadDelay() const final override;

	// Projectile
	bool HasHitWall() const;
	bool HasHitActor() const;
	virtual int GetDamage() const final override;
	virtual float GetMaxRange() const final override;
	virtual float GetRotationRate() const final override;
	virtual float GetDyingRotation() const final override;

	// Both
	virtual const std::string GetClassName() const final override;
};

#endif