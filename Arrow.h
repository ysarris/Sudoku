#pragma once
#ifndef ARROW_H
#define ARROW_H

#include "Projectile.h"

class Arrow : public Projectile
{
public:
	Arrow(sf::Vector2f startPos, Direction facing);
	virtual void HitWall(sf::Vector2f hitPos) final override;

private:
	virtual sf::Vector2f GetSpawnOffset() const final override;
	virtual int GetDamage() const final override;
	virtual float GetDyingRotation() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif