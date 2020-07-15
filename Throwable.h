#pragma once
#ifndef THROWABLE_H
#define THROWABLE_H

#include "Weapon.h"
#include "Projectile.h"

static constexpr auto THROWABLE_THROW_FILENAME = "Throw";
static constexpr auto THROWABLE_HIT_FLOOR_FILENAME = "HitFloor";
static constexpr auto THROWABLE_HIT_WALL_DROP_SPEED = 60.0f; // used as primary speed for vertical primary movement
															 // and secondary speed for horizontal primary movement

class Throwable : public Weapon, public Projectile
{
public:
	// Projectile
	float GetDistanceFromFloor() const;
	bool IsReadyForFloorCollision() const;
	virtual void HitFloor();
	virtual void HitWall(sf::Vector2f hitPos) override;

	// Weapon
	virtual bool IsSilent() const final override;

	// Both
	virtual void Pause() final override;
	virtual void Resume() final override;
	virtual void Update(float dtAsSeconds) override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;

protected:
	// Projectile
	Throwable(sf::Vector2f startPos, float dyingTimer, Direction primMov, float floorCoordinate);
	float GetFloorCoordinate() const;
	void WallDrop();
	void WallBounce();
	void FloorBounce();
	void SetFloorCoordinate(float value);
	void MakeTrajectoryParabolic(float initialHeight, float maxHeight);
	
	// Weapon
	Throwable(int weaponID);
	
	// Both
	~Throwable() noexcept = default;
	bool IsWeapon() const;

private:
	// Projectile
	Timer m_FloorHitTimer;
	float m_FloorCoordinate;

	void ApplyGravity(float dtAsSeconds);
	float GetBottomAdjustment() const;	// Returns adjustment to translate measurement or coordinate to bottom of throwable (subtract from measurement, add to coordinate)
	virtual void ReachedMaxDistance() final override;
	virtual sf::Vector2f GetSpawnOffset() const final override;
	virtual float GetSpawnRotation() const final override;

	// Weapon
	virtual void Throw(Direction facing, float floorCoordinate, float startHeight) = 0;
	virtual void CreateProjectile(Direction facing, float floorCoordinate, float startHeight) final override;
	virtual bool GetLoadedAmmoVisisble() const final override;
};

#endif