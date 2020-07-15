#include "Player.h"
#include <sstream>				// for stringstream
#include "Random.h"				// for TakeRandomElementFromSet()
#include "Mathematics.h"		// for HalfOf()
#include "TextCharacters.h"		// for SPACE_CHAR
#include "Weapons_info.h"		// for weapon IDs and classes

static constexpr auto PLAYER_CLASS_NAME = "Player";
static constexpr auto PLAYER_SWAP_SOUND_FILENAME = "Swap";
static constexpr auto PLAYER_NOISE_TIME = 1.0f;
static constexpr auto PLAYER_MAX_HEALTH = 1000.0f;
static constexpr auto PLAYER_PRIMARY_SPEED = 105.0f;
static constexpr auto PLAYER_SECONDARY_SPEED = 90.0f;
static constexpr auto PLAYER_DEFAULT_FACING = Direction::DOWN;
static constexpr auto PLAYER_DEFAULT_EQUIPPED_WEAPON_INDEX = 0;
static constexpr auto PLAYER_DEFAULT_SHOOTING = false;
static constexpr auto PLAYER_HEARING_DISTANCE = 450.0f;
static constexpr auto PLAYER_WEAPON_HOLD_OFFSET_DIRECTION = Direction::RIGHT; // use a horizontal direction for better accuracy

// Returns the ID of a weapon the player doesnt already own
static int ChooseRandomNewWeapon(std::string ownedWeapons)
{
	// Start off with all weapon IDs
	std::set<int> newWeaponIDs = ALL_WEAPON_IDS;

	// Remove owned weapons from set
	std::stringstream ss;
	ss << ownedWeapons;
	int weaponToRemove;

	while (!ss.eof())
	{
		ss >> weaponToRemove;
		newWeaponIDs.erase(weaponToRemove);
	}
	ss.clear();

	auto newWeaponsLeft = static_cast<int>(newWeaponIDs.size());

	// Any weapons left to unlock ?
	if (newWeaponsLeft > 0) { return TakeRandomElementFromSet(newWeaponIDs); }
	else { Assert(ERROR_ALL_WEAPONS_UNLOCKED); }
}

Player::Player(sf::Vector2f spawnPos) :
	Object(spawnPos),
	Actor(spawnPos, PLAYER_MAX_HEALTH, PLAYER_PRIMARY_SPEED, PLAYER_SECONDARY_SPEED),
	m_EquippedWeaponIndex(PLAYER_DEFAULT_EQUIPPED_WEAPON_INDEX),
	m_NoiseTimer(),
	m_UsingWeapon(PLAYER_DEFAULT_SHOOTING)
{
}

void Player::SetUsingWeapon(bool value)
{
	m_UsingWeapon = value;
	// Stop sound for certain weapons when out of ammo or shoot key released (e.g. flamethrower)
	if (value == false) { GetEquippedWeapon()->StopUsing(); }
}

void Player::ResetNewGame(int starterWeaponID)
{
	RemoveAllWeapons();
	AddNewWeapon(starterWeaponID);
}

void Player::ResetNewLevel()
{
	// Stop moving, using weapon and making noise
	StopAllActions();
	m_NoiseTimer.Reset();

	// Restore health and place at spawn position
	Revive();

	// Reload all and equip default weapon
	for (auto& weapon : m_Weapons) { weapon->Reload(); }
	m_EquippedWeaponIndex = PLAYER_DEFAULT_EQUIPPED_WEAPON_INDEX;
}

void Player::SwapWeapon()
{
	// Must have more than 1 weapon to swap
	if (static_cast<int>(m_Weapons.size()) > 1)
	{
		GetEquippedWeapon()->StopUsing();

		// Equip next weapon, loop back to start if end reached
		m_EquippedWeaponIndex = ++m_EquippedWeaponIndex % static_cast<int>(m_Weapons.size());

		// Move newly equipped weapon to player's position
		GetEquippedWeapon()->Hold(GetFacing(), GetPos());

		PlaySound(PLAYER_SWAP_SOUND_FILENAME, mp_WeaponSwapSound);
	}
}

void Player::AddNewWeapon(const int ID)
{
	switch (ID)
	{
	case PISTOL_ID:
		m_Weapons.emplace_back(std::make_unique<Pistol>());
		break;
	case HATCHET_ID:
		m_Weapons.emplace_back(std::make_unique<Hatchet>());
		break;
	case SHOTGUN_ID:
		m_Weapons.emplace_back(std::make_unique<Shotgun>());
		break;
	case GRENADE_ID:
		m_Weapons.emplace_back(std::make_unique<Grenade>());
		break;
	case BOW_ID:
		m_Weapons.emplace_back(std::make_unique<Bow>());
		break;
	case ROCKETLAUNCHER_ID:
		m_Weapons.emplace_back(std::make_unique<RocketLauncher>());
		break;
	case FLAMETHROWER_ID:
		m_Weapons.emplace_back(std::make_unique<Flamethrower>());
		break;
	case FIRE_BOTTLE_ID:
		m_Weapons.emplace_back(std::make_unique<FireBottle>());
		break;
	default:
		Assert(ERROR_UNKNOWN_WEAPON_ID);
		break;
	}
	// Equip the new weapon
	m_EquippedWeaponIndex = static_cast<int>(m_Weapons.size()) - 1;
}

void Player::AddRandomNewWeapon()
{
	AddNewWeapon(ChooseRandomNewWeapon(GetOwnedWeaponIDs()));
}

void Player::RemoveAllWeapons()
{
	m_Weapons.clear();
}

void Player::StopAllActions()
{
	SetUsingWeapon(PLAYER_DEFAULT_SHOOTING);
	SetPrimaryMovement(DEFAULT_DIRECTION);
	SetSecondaryMovement(DEFAULT_DIRECTION);
}

bool Player::IsMakingNoise() const
{
	return !m_NoiseTimer.RanOut();
}

bool Player::IsUsingWeapon() const
{
	return m_UsingWeapon;
}

float Player::GetHearingDistance() const
{
	return PLAYER_HEARING_DISTANCE;
}

std::string Player::GetOwnedWeaponIDs()
{
	std::string weaponIDs;
	
	for (auto& weapon : m_Weapons)
	{
		weaponIDs.append(std::to_string(weapon->GetID()));
		// Dont add space after last weapon
		if (weapon != m_Weapons.back()) { weaponIDs.push_back(SPACE_CHAR); }
	}
	return weaponIDs;
}

float Player::GetMaxHealth() const
{
	return PLAYER_MAX_HEALTH;
}

void Player::HitWall(sf::Vector2f hitPos)
{
	Movable::HitWall(hitPos);
	// Move weapon with player after hitting wall
	GetEquippedWeapon()->Hold(GetFacing(), GetPos());
}

void Player::Pause()
{
	// Stop using weapon and moving
	StopAllActions();
	StopSound(); // footstep sound
	PauseSound(mp_WeaponSwapSound);
	for (auto& weapon : m_Weapons) { weapon->Pause(); }
}

void Player::Resume()
{
	ResumeSound(mp_WeaponSwapSound);
	for (auto& weapon : m_Weapons) { weapon->Resume(); }
}

void Player::Update(float dtAsSeconds)
{
	// Default actor updates
	Actor::Update(dtAsSeconds);

	// Player updates
	if (GetStatus() == HealthStatus::ALIVE)
	{
		if (IsMakingNoise()) { m_NoiseTimer.Decrement(dtAsSeconds); }
		if (GetEquippedWeapon()->IsLoaded() && m_UsingWeapon) { UseWeapon(); }
		GetEquippedWeapon()->Hold(GetFacing(), GetPos());
		GetEquippedWeapon()->Update(dtAsSeconds);
	}
}

void Player::Draw(sf::RenderWindow& rWindow) const
{
	// Draw the player behind weapon unless facing UP and weapon is a throwable
	auto playerBehindWeapon = !(GetFacing() == Direction::UP && GetEquippedWeapon()->IsThrowable());
	if (playerBehindWeapon) { Object::Draw(rWindow); }
	// Only draw the weapon if player is alive
	if (Player::GetStatus() == HealthStatus::ALIVE) { GetEquippedWeapon()->Draw(rWindow); }
	if (!playerBehindWeapon) { Object::Draw(rWindow); }
}

void Player::UseWeapon()
{
	GetEquippedWeapon()->Use(GetFacing(), GetFeetCoordinate(), GetWeaponHeightAboveFloor());
	// Made noise ?
	if (!GetEquippedWeapon()->IsSilent()) { m_NoiseTimer.Reset(PLAYER_NOISE_TIME); }
	// Ammo ran out ?
	if (GetEquippedWeapon()->IsOutOfAmmo()) { SetUsingWeapon(PLAYER_DEFAULT_SHOOTING); }
}

float Player::GetWeaponHeightAboveFloor()
{
	// Use same offset for all directions so height is equal
	// Negate hold offset, because going UP in the world decrements Y
	// (negative offset increases hold height / positive offset lowers hold height)
	return HalfOf(GetHeight()) - GetEquippedWeapon()->GetHoldOffset(PLAYER_WEAPON_HOLD_OFFSET_DIRECTION).y;
}

Weapon* Player::GetEquippedWeapon() const
{
	return (m_Weapons.at(m_EquippedWeaponIndex).get());
}

const std::string Player::GetClassName() const
{
	return PLAYER_CLASS_NAME;
}