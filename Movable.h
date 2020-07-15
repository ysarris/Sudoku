#pragma once
#ifndef MOVABLE_H
#define MOVABLE_H

#include "Object.h"
#include "Direction.h"
#include "Timer.h"

enum class HealthStatus { ALIVE, DYING, DEAD };
static constexpr auto DEFAULT_HEALTH_STATUS = HealthStatus::ALIVE;
static constexpr auto DEFAULT_DIRECTION = Direction::NONE;
static constexpr auto DEFAULT_SPEED = 0.0f;

class Movable : virtual public Object
{
public:
	HealthStatus GetStatus() const;
	Direction GetPrimaryMovement() const;
	Direction GetSecondaryMovement() const;
	float GetDyingTimeLeft() const;
	void SetPrimaryMovement(Direction dir);
	void SetSecondaryMovement(Direction dir);
	virtual bool IsReadyForCollision() const;
	virtual bool IsReadyForWallCollision() const;
	virtual void HitWall(sf::Vector2f hitPos);
	virtual void Draw(sf::RenderWindow& rWindow) const override;

protected:
	Timer m_HitTimer;

	Movable(sf::Vector2f startPos, float dyingTimer, Direction primMov, float primSpeed, Direction secMov, float secSpeed);
	~Movable() noexcept = default;
	float GetPrimarySpeed() const;
	float GetSecondarySpeed() const;
	void SetPrimarySpeed(float speed);
	void SetSecondarySpeed(float speed);
	void SetHealthStatus(HealthStatus status);
	void TurnOppositeDirPrim();
	void TurnOppositeDirSec();
	void StartDying(std::string filename);
	void SetDyingTimer(float value);
	virtual float GetHitDelay() const;
	virtual void Move(float dtAsSeconds);
	virtual void Update(float dtAsSeconds) override;

private:
	Direction m_PrimaryMovement;
	Direction m_SecondaryMovement;
	float m_PrimarySpeed;
	float m_SecondarySpeed;
	HealthStatus m_HealthStatus;
	Timer m_DyingTimer;

	virtual float GetDyingRotation() const = 0;
};

#endif