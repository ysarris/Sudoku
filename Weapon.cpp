#include "Weapon.h"
#include "Throwable.h"	// for dynamic_cast
#include "Shooter.h"	// for dynamic_cast

static constexpr auto WEAPONS_CLASS_NAME = "Weapon";
static constexpr auto WEAPON_DEFAULT_SILENT = false;
static constexpr auto WEAPON_DEFAULT_AMMO_VISIBLE = false;
static constexpr auto WEAPON_DEFAULT_CAPACITY = 1;
static constexpr auto WEAPON_RELOAD_FILENAME = "Reload";
static constexpr auto WEAPON_DEFAULT_FILENAME = "DEFAULT";
static constexpr auto WEAPON_UNLOADED_FILENAME_SUFFIX = "_UNLOADED";

void Weapon::SetProjectiles(std::vector<std::unique_ptr<Projectile>>* pProjectiles)
{
	msp_Projectiles = pProjectiles;
}

int Weapon::GetID() const
{
	return m_ID;
}

// Indicates whether a weapon is a throwable (e.g. grenade, hatchet)
bool Weapon::IsThrowable() const
{
	return dynamic_cast<const Throwable*>(this) != nullptr ;
}

// Indicates whether a weapon is a shooter (e.g. pistol, bow, flamethrower)
bool Weapon::IsShooter() const
{
	return dynamic_cast<const Shooter*>(this) != nullptr ;
}

bool Weapon::IsOutOfAmmo() const
{
	return !(m_AmmoCount > 0);
}

void Weapon::Use(Direction facing, float floorCoordinate, float startHeight)
{
	CreateProjectile(facing, floorCoordinate, startHeight);
	m_AmmoCount--;
	// Start reload process ? 
	if (IsOutOfAmmo()) { m_ReloadTimer.Reset(GetReloadDelay()); }
}

void Weapon::StopUsing()
{
	// Let sounds not on loop finish playing (e.g. pistol shot, hatchet throw)
	if (SoundIsOnLoop()) { StopSound(); }
}

void Weapon::Hold(Direction facing, sf::Vector2f playerPos)
{
	OrientSprite(facing);

	// Default position adjustment
	SetPos(playerPos + GetHoldOffset(facing));

	// Additional reload position adjustments
	if (IsReloading()) { SetPos(GetPos() + GetReloadHoldOffset(facing)); }
}

bool Weapon::IsSilent() const
{
	return WEAPON_DEFAULT_SILENT;
}

bool Weapon::IsLoaded() const
{
	return IsReloading() ? false : true;
}

void Weapon::Reload()
{
	m_ReloadTimer.Reset();
	m_AmmoCount = GetCapacity();
}

void Weapon::Pause()
{
	PauseSound();
	PauseSound(mp_ReloadSound);
}

void Weapon::Resume()
{
	ResumeSound();
	ResumeSound(mp_ReloadSound);
}

void Weapon::Update(float dtAsSeconds)
{
	if (IsReloading())
	{
		// Hasnt reloaded yet ?
		if (!m_ReloadTimer.RanOut())
		{
			m_ReloadTimer.Decrement(dtAsSeconds);

			// Time to reload ?
			if (m_ReloadTimer.RanOut())
			{
				PlaySound(WEAPON_RELOAD_FILENAME, mp_ReloadSound);
				Reload();
			}
		}
		// else { Already reloaded and waiting for reload sound to finish playing }
	}
}

// Weapon
Weapon::Weapon(int ID) :
	m_ID(ID),
	m_ReloadTimer(),
	m_AmmoCount()
{
	// Doesnt add "Weapon/" to throwable projectile directory
	if (ID != WEAPON_DEFAULT_ID) { AddToDirectory(WEAPONS_CLASS_NAME); }
}

float Weapon::GetHoldRotation(Direction facing) const
{
	return NO_ROTATION;
}

void Weapon::OrientSprite(Direction facing)
{
	auto filename = GetDirectionString((facing == Direction::LEFT) ? GetOppositeDirection(facing) : facing);

	// For weapons that use the same sprite for multiple directions (e.g. bow, grenade, hatchet)
	// use the default filename if the specified file doesnt exist
	if (!ImageExists(filename)) { filename = WEAPON_DEFAULT_FILENAME; }

	// For weapons with visible ammo that are not also a projectile (e.g. rocketlauncher, bow, flamethrower)
	// and currently not loaded add suffix to the filename
	if (GetLoadedAmmoVisisble() && !IsThrowable() && IsReloading()) { filename.append(WEAPON_UNLOADED_FILENAME_SUFFIX); }

	SetSprite(filename);
	RotateSpriteBy(GetHoldRotation(facing));

	// Flip right facing sprite to left and up facing throwables so they look turned away (e.g. the grenade safety lever)
	if (facing == Direction::LEFT || (facing == Direction::UP && IsThrowable())) { FlipSpriteHorizontally(); }
}

bool Weapon::IsReloading() const
{
	return !m_ReloadTimer.RanOut() || SoundIsPlaying(mp_ReloadSound) || IsOutOfAmmo();
}

sf::Vector2f Weapon::GetReloadHoldOffset(Direction facing) const
{
	return NO_OFFSET;
}

bool Weapon::GetLoadedAmmoVisisble() const
{
	return WEAPON_DEFAULT_AMMO_VISIBLE;
}

int Weapon::GetCapacity() const
{
	return WEAPON_DEFAULT_CAPACITY;
}