#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"
#include "Weapon.h"
#include "Timer.h"

class Player : public Actor
{
public:
	Player(sf::Vector2f spawnPos);
	void SetUsingWeapon(bool value);
	void ResetNewGame(int starterWeaponID);
	void ResetNewLevel();
	void SwapWeapon();
	void AddNewWeapon(const int ID);
	void AddRandomNewWeapon();
	void RemoveAllWeapons();
	void StopAllActions();
	bool IsMakingNoise() const;
	bool IsUsingWeapon() const;
	float GetHearingDistance() const;
	std::string GetOwnedWeaponIDs();
	virtual float GetMaxHealth() const final override;
	virtual void HitWall(sf::Vector2f hitPos) final override;
	virtual void Pause() final override;
	virtual void Resume() final override;
	virtual void Update(float dtAsSeconds) final override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;

private:
	std::vector<std::unique_ptr<Weapon>> m_Weapons;
	std::unique_ptr<sf::Sound> mp_WeaponSwapSound;
	int m_EquippedWeaponIndex;
	Timer m_NoiseTimer;
	bool m_UsingWeapon;

	void UseWeapon();
	float GetWeaponHeightAboveFloor();
	Weapon* GetEquippedWeapon() const;
	virtual const std::string GetClassName() const final override;
};

#endif