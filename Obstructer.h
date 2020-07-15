#pragma once
#ifndef OBSTRUCTER_H
#define OBSTRUCTER_H

#include "Enemy.h"
#include "Timer.h"

class Obstructer : public Enemy
{
public:
	Obstructer(sf::Vector2f startPos);
	virtual int GetKillPoints() const final override;
	virtual float GetMaxHealth() const final override;
	virtual void HitByProjectile(int damage, bool isExploding) final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	Timer m_GrowthTimer;

	virtual bool HasHealthBar() const final override;
	virtual int GetAttackDamage() const final override;
	virtual float GetHearingDistance() const final override;;
	virtual float GetForwardVisionDistance() const final override;;
	virtual float GetPeripheralVisionDistance() const final override;;
	virtual const std::string GetClassName() const final override;
};

#endif