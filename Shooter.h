#pragma once
#ifndef SHOOTER_H
#define SHOOTER_H

#include "Weapon.h"

static constexpr auto SOUND_FILENAME_SHOT = "shot";

class Shooter : public Weapon
{
public:
	virtual bool IsLoaded() const final override;
	virtual void Reload() final override;
	virtual void Update(float dtAsSeconds) override;

protected:
	Shooter(int weaponID);
	~Shooter() noexcept = default;

private:
	Timer m_ShotTimer;

	bool IsBetweenShots() const;
	virtual void Shoot(Direction facing) = 0;
	virtual float GetShotDelay() const;
	virtual void CreateProjectile(Direction facing, float floorCoordinate, float startHeight) final override;
};

#endif