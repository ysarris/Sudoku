#pragma once
#ifndef WANDERER_H
#define WANDERER_H

#include "Enemy.h"

class Wanderer : public Enemy
{
public:
	Wanderer(sf::Vector2f startPos, Direction primMov);
	void Wander();
	virtual int GetKillPoints() const final override;
	virtual float GetMaxHealth() const final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	virtual int GetAttackDamage() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif