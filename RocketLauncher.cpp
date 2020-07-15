#include "RocketLauncher.h"
#include "Rocket.h"

static constexpr auto ROCKETLAUNCHER_CLASS_NAME = "Rocket Launcher";
static constexpr auto ROCKETLAUNCHER_RELOAD_DELAY = 3.5f;
static constexpr auto ROCKETLAUNCHER_AMMO_VISIBLE = true;
static inline const auto ROCKETLAUNCHER_HOLD_OFFSET_UP = sf::Vector2f(8, -13);
static inline const auto ROCKETLAUNCHER_HOLD_OFFSET_DOWN = sf::Vector2f(-8, 5);
static inline const auto ROCKETLAUNCHER_HOLD_OFFSET_LEFT = sf::Vector2f(-6, 3);
static inline const auto ROCKETLAUNCHER_HOLD_OFFSET_RIGHT = sf::Vector2f(6, 3);
static inline const auto ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_UP = sf::Vector2f(0, 7);
static inline const auto ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_DOWN = sf::Vector2f(0, -7);
static inline const auto ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_LEFT = sf::Vector2f(9, 0);
static inline const auto ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_RIGHT = sf::Vector2f(-9, 0);

RocketLauncher::RocketLauncher() :
	Shooter(ROCKETLAUNCHER_ID)
{
}

void RocketLauncher::Shoot(Direction facing)
{
	PlaySound(SOUND_FILENAME_SHOT);
	msp_Projectiles->emplace_back(std::make_unique<Rocket>(GetPos(), facing));
}

sf::Vector2f RocketLauncher::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return ROCKETLAUNCHER_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return ROCKETLAUNCHER_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return ROCKETLAUNCHER_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return ROCKETLAUNCHER_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

sf::Vector2f RocketLauncher::GetReloadHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return ROCKETLAUNCHER_RELOAD_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float RocketLauncher::GetReloadDelay() const
{
	return ROCKETLAUNCHER_RELOAD_DELAY;
}

bool RocketLauncher::GetLoadedAmmoVisisble() const
{
	return ROCKETLAUNCHER_AMMO_VISIBLE;
}

const std::string RocketLauncher::GetClassName() const
{
	return ROCKETLAUNCHER_CLASS_NAME;
}