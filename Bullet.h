#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet(sf::Vector2f startPos, Direction facing);

private:
	virtual sf::Vector2f GetSpawnOffset() const final override;
	virtual int GetDamage() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif