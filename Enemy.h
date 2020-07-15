#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"
#include "Player.h"
#include "Timer.h"
#include "HealthBar.h"

class Enemy : public Actor
{
public:
	static void SetPlayer(Player* pPlayer);
	bool IsOutOfGate() const;
	bool IsOutOfGround() const;
	void AttackPlayer();
	virtual ~Enemy() noexcept = default;
	virtual int GetKillPoints() const = 0;
	virtual bool IsReadyForCollision() const final override;
	virtual bool IsReadyForWallCollision() const final override;
	virtual void HitWall(sf::Vector2f hitPos) override;
	virtual void Update(float dtAsSeconds) override;
	virtual void Draw(sf::RenderWindow& rWindow) const override;

protected:
	static inline Player* msp_Player;
	Timer m_DirectionChangeTimer;

	Enemy(sf::Vector2f startPos, Direction primMov, float maxHealth, float primSpeed = DEFAULT_SPEED, float secSpeed = DEFAULT_SPEED);
	void SpawnFromGate();
	void SpawnFromGround();
	void TurnTowards(sf::Vector2f pos);
	void MoveTowards(sf::Vector2f pos, float speed);
	bool CanSeePlayer() const;
	bool CanHearPlayer() const;
	std::pair<Direction, Direction> WhereIs(sf::Vector2f pos) const;

private:
	static inline sf::Sprite ms_GroundLine;
	static inline sf::Sound ms_GroundSound;
	bool m_SeePlayer;
	bool m_HearPlayer;
	bool m_OutOfGate;
	bool m_OutOfGround;
	float m_FinalHeight;
	Timer m_GroundSpawnTimer;
	std::unique_ptr<HealthBar> mp_HealthBar;

	void ExitGate();
	void ExitGround(float dtAsSeconds);
	void LookForPlayer();
	void ListenForPlayer();
	float GetMinDistanceToExitGate() const;
	virtual bool HasHealthBar() const;
	virtual int GetAttackDamage() const = 0;
	virtual float GetHearingDistance() const;
	virtual float GetForwardVisionDistance() const;
	virtual float GetPeripheralVisionDistance() const;
	virtual const std::string GetClassName() const override;
};

#endif