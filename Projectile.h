#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Movable.h"
#include "Actor.h"
#include "Timer.h"

static constexpr auto PROJECTILE_FILENAME_HIT_ACTOR = "HitActor";
static constexpr auto PROJECTILE_FILENAME_HIT_WALL = "HitWall";
static constexpr auto PROJECTILE_FILENAME_EXPLODE = "Explode";
static constexpr auto PROJECTILE_TRAJECTORY_HEIGHT_LIMIT = 30.0f;
static constexpr auto PROJECTILE_MAX_SPEED = 400;
static constexpr auto THROWABLE_MAX_SPEED = 250;
static constexpr auto ERROR_PROJECTILE_MAX_RANGE_NEGATIVE = "Projectile max range cant be negative.";

struct AttachedToActor
{
	Actor* pActor = nullptr;
	bool startedDying = false;
	sf::Vector2f offset = NO_OFFSET;

	AttachedToActor(Actor* _pActor, bool _startedDying, sf::Vector2f _offset) :
		pActor(_pActor),
		startedDying(_startedDying),
		offset(_offset) {}
};

class Projectile : public Movable
{
public:
	float GetDistanceLeftToTravel() const;
	bool IsExploding() const;
	virtual ~Projectile() noexcept = default;
	virtual void HitActor(Actor& actor);
	virtual bool IsReadyForWallCollision() const final override;
	virtual bool IsReadyForCollision() const final override;
	virtual void HitWall(sf::Vector2f hitPos) override;
	virtual void Update(float dtAsSeconds) override;
	virtual void Draw(sf::RenderWindow& rWindow) const override;

protected:
	Timer m_WallHitTimer;

	Projectile(sf::Vector2f startPos, float dyingTimer, Direction primMov, float primSpeed = DEFAULT_SPEED);
	Projectile(); // for weapon instance of a throwable (e.g. when player unlocks grenade or hatchet weapon)
	float GetDistanceTravelled() const;
	void AdjustSpawnPositionAndRotation();
	void MakeTrajectorySpread(float minPrimarySpeed, float maxPrimarySpeed, float maxAbsoluteSecondarySpeed);
	virtual sf::Vector2f GetSpawnOffset() const;
	virtual float GetSpawnRotation() const;
	virtual float GetMaxRange() const;
	virtual float GetHitDelay() const final override;

private:
	float m_DistanceTravelled; // the displacement on y-axis for vertical primary movement and x-axis for horizontal primary movement
	std::unique_ptr<AttachedToActor> mp_Attach;

	void UpdateDistanceTravelled(sf::Vector2f oldPos);
	virtual void ReachedMaxDistance();
	virtual bool GetIsExplosive() const;
	virtual int GetDamage() const = 0;
	virtual float GetRotationRate() const;
	virtual float GetDyingRotation() const override;
	virtual void Move(float dtAsSeconds) final override;
};

#endif