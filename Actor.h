#pragma once
#ifndef ACTOR_H
#define ACTOR_H

#include "Movable.h"
#include "Timer.h"
#include "Direction.h"

static constexpr auto ACTOR_DEFAULT_DYING_ROTATION = 90.0f;

class Actor : public Movable
{
public:
	float GetHealth() const;
	float GetFeetCoordinate();
	void TakeDamage(int value, bool resetHitTimer = true);
	void RestoreHealth(float value);
	void StunFor(float value);
	virtual float GetMaxHealth() const = 0;
	virtual void HitByProjectile(int damage, bool isExploding);
	virtual void Update(float dtAsSeconds) override;

protected:
	sf::Vector2f m_SpawnPos;

	Actor(sf::Vector2f startPos, float maxHealth, float primSpeed = DEFAULT_SPEED, float secSpeed = DEFAULT_SPEED, Direction primMov = DEFAULT_DIRECTION, Direction secMov = DEFAULT_DIRECTION);
	~Actor() noexcept = default;
	void Revive();
	void OrientSprite();
	void SetFacing(Direction dir);
	Direction GetFacing() const;

private:
	float m_Health;
	Direction m_Facing;
	int m_AnimFrame;
	std::unique_ptr<sf::Sound> mp_HitSound;
	Timer m_AnimTimer;
	Timer m_StunTimer;

	virtual void Move(float dtAsSeconds) final override;
	virtual float GetDyingRotation() const final override;
};

#endif