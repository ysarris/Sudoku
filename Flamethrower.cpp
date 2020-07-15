#include "Flamethrower.h"
#include "Flame.h"

static constexpr auto FLAMETHROWER_CLASS_NAME = "Flamethrower";
static constexpr auto FLAMETHROWER_SHOT_DELAY = 0.1f;
static constexpr auto FLAMETHROWER_RELOAD_DELAY = 2.0f;
static constexpr auto FLAMETHROWER_AMMO_VISIBLE = true;
static constexpr auto FLAMETHROWER_CAPACITY = 100;
static constexpr auto FLAMETHROWER_FLAMES_PER_SHOT = 4;
static inline const auto FLAMETHROWER_HOLD_OFFSET_UP = sf::Vector2f(0, -19);
static inline const auto FLAMETHROWER_HOLD_OFFSET_DOWN = sf::Vector2f(0, 12);
static inline const auto FLAMETHROWER_HOLD_OFFSET_LEFT = sf::Vector2f(-15, 6);
static inline const auto FLAMETHROWER_HOLD_OFFSET_RIGHT = sf::Vector2f(15, 6);
static inline const auto FLAMETHROWER_RELOAD_HOLD_OFFSET_UP_DOWN = sf::Vector2f(0, 0);
static inline const auto FLAMETHROWER_RELOAD_HOLD_OFFSET_LEFT_RIGHT = sf::Vector2f(0, -2);

Flamethrower::Flamethrower() :
	Shooter(FLAMETHROWER_ID)
{
}

void Flamethrower::Shoot(Direction facing)
{
	if (!SoundIsPlaying()) { PlaySound(SOUND_FILENAME_SHOT, true); }
	for (auto f = 0; f < FLAMETHROWER_FLAMES_PER_SHOT; f++)
		msp_Projectiles->emplace_back(std::make_unique<Flame>(GetPos(), facing));
}

sf::Vector2f Flamethrower::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return FLAMETHROWER_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return FLAMETHROWER_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return FLAMETHROWER_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return FLAMETHROWER_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

sf::Vector2f Flamethrower::GetReloadHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
	case Direction::DOWN:
		return FLAMETHROWER_RELOAD_HOLD_OFFSET_UP_DOWN;
		break;
	case Direction::LEFT:
	case Direction::RIGHT:
		return FLAMETHROWER_RELOAD_HOLD_OFFSET_LEFT_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Flamethrower::GetShotDelay() const
{
	return FLAMETHROWER_SHOT_DELAY;
}

float Flamethrower::GetReloadDelay() const
{
	return FLAMETHROWER_RELOAD_DELAY;
}

bool Flamethrower::GetLoadedAmmoVisisble() const
{
	return FLAMETHROWER_AMMO_VISIBLE;
}

int Flamethrower::GetCapacity() const
{
	return FLAMETHROWER_CAPACITY;
}

const std::string Flamethrower::GetClassName() const
{
	return FLAMETHROWER_CLASS_NAME;
}