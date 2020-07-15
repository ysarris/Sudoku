#include "Shooter.h"

static constexpr auto SHOOTER_CLASS_NAME = "Shooter";
static constexpr auto SHOOTER_DEFAULT_SHOT_DELAY = 0.5f;

bool Shooter::IsLoaded() const
{
	return !IsBetweenShots() && Weapon::IsLoaded();
}

void Shooter::Reload()
{
	Weapon::Reload();
	m_ShotTimer.Reset();
}

void Shooter::Update(float dtAsSeconds)
{
	if (IsBetweenShots()) { m_ShotTimer.Decrement(dtAsSeconds); }
	// Default weapon updates
	else { Weapon::Update(dtAsSeconds); }
}

Shooter::Shooter(int weaponID) :
	Weapon(weaponID),
	m_ShotTimer()
{
	AddToDirectory(SHOOTER_CLASS_NAME);
}

// Too soon to shoot again ?
bool Shooter::IsBetweenShots() const
{
	return !m_ShotTimer.RanOut();
}

float Shooter::GetShotDelay() const
{
	return SHOOTER_DEFAULT_SHOT_DELAY;
}

void Shooter::CreateProjectile(Direction facing, float floorCoordinate, float startHeight)
{
	Shoot(facing);
	m_ShotTimer.Reset(GetShotDelay());
}