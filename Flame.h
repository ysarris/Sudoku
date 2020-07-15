#pragma once
#ifndef FLAME_H
#define FLAME_H

#include "Projectile.h"
#include "SizeStatus.h"

class Flame : public Projectile
{
public:
	Flame(sf::Vector2f startPos, Direction facing);
	~Flame();
	void Burn(float time = DEFAULT_TIMER_VALUE);
	virtual void HitWall(sf::Vector2f hitPos) final override;
	virtual void HitActor(Actor& actor) final override;
	virtual void Pause() final override;
	virtual void Resume() final override;
	virtual void Update(float dtAsSeconds) final override;

private:
	static inline std::unique_ptr<sf::Sound> msp_BurnSound;
	static inline int ms_Count = 0;
	SizeStatus m_SizeStatus;
	Timer m_ScaleTimer;

	virtual void ReachedMaxDistance() final override;
	virtual sf::Vector2f GetSpawnOffset() const final override;
	virtual float GetMaxRange() const final override;
	virtual bool GetIsExplosive() const final override;
	virtual int GetDamage() const final override;
	virtual const std::string GetClassName() const final override;
};

#endif