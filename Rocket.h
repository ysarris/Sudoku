#pragma once
#ifndef ROCKET_H
#define ROCKET_H

#include "Projectile.h"

class Rocket : public Projectile
{
public:
	Rocket(sf::Vector2f startPos, Direction facing);
	virtual void HitWall(sf::Vector2f hitPos) final override;
	virtual void HitActor(Actor& actor) final override;

private:
	virtual sf::Vector2f GetSpawnOffset() const final override;
	virtual bool GetIsExplosive() const final override;
	virtual int GetDamage() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif