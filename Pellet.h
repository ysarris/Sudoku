#pragma once
#ifndef PELLET_H
#define PELLET_H

#include "Projectile.h"

class Pellet : public Projectile
{
public:
	Pellet(sf::Vector2f startPos, Direction facing);

private:
	virtual sf::Vector2f GetSpawnOffset() const final override;
	virtual float GetMaxRange() const final override;
	virtual int GetDamage() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif