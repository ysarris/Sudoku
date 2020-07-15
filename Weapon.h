#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "Object.h"
#include "Direction.h"
#include "Projectile.h"
#include "Timer.h"

static constexpr auto WEAPON_DEFAULT_ID = 0;

class Weapon : virtual public Object
{
public:
	static void SetProjectiles(std::vector<std::unique_ptr<Projectile>>* pProjectiles);
	int GetID() const;
	bool IsThrowable() const;
	bool IsShooter() const;
	bool IsOutOfAmmo() const;
	void Use(Direction facing, float floorCoordinate, float startHeight);
	void StopUsing();
	void Hold(Direction facing, sf::Vector2f playerPos);
	virtual ~Weapon() noexcept = default;
	virtual sf::Vector2f GetHoldOffset(Direction facing) const = 0;
	virtual bool IsSilent() const;
	virtual bool IsLoaded() const;
	virtual void Reload();
	virtual void Pause() override;
	virtual void Resume() override;
	virtual void Update(float dtAsSeconds) override;

protected:
	static inline std::vector<std::unique_ptr<Projectile>>* msp_Projectiles;

	Weapon(int ID = WEAPON_DEFAULT_ID);
	virtual float GetHoldRotation(Direction facing) const;

private:
	const int m_ID;
	Timer m_ReloadTimer;
	int m_AmmoCount;
	std::unique_ptr<sf::Sound> mp_ReloadSound;

	void OrientSprite(Direction facing);
	bool IsReloading() const;
	virtual void CreateProjectile(Direction facing, float floorCoordinate, float startHeight) = 0;
	virtual sf::Vector2f GetReloadHoldOffset(Direction facing) const;
	virtual float GetReloadDelay() const = 0;
	virtual bool GetLoadedAmmoVisisble() const;
	virtual int GetCapacity() const;
};

#endif