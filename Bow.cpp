#include "Bow.h"
#include "Arrow.h"

static constexpr auto BOW_CLASS_NAME = "Bow";
static constexpr auto BOW_SILENT = true;
static constexpr auto BOW_RELOAD_DELAY = 0.2f;
static constexpr auto BOW_AMMO_VISIBLE = true;
static constexpr auto BOW_HOLD_ROTATION_DOWN = 90.0f;
static inline const auto BOW_HOLD_OFFSET_UP = sf::Vector2f(0, -7);
static inline const auto BOW_HOLD_OFFSET_DOWN = sf::Vector2f(0, 6);
static inline const auto BOW_HOLD_OFFSET_LEFT = sf::Vector2f(-6, 3);
static inline const auto BOW_HOLD_OFFSET_RIGHT = sf::Vector2f(6, 3);
static inline const auto BOW_RELOAD_HOLD_OFFSET_UP = sf::Vector2f(0, 8);
static inline const auto BOW_RELOAD_HOLD_OFFSET_DOWN = sf::Vector2f(0, -1);
static inline const auto BOW_RELOAD_HOLD_OFFSET_LEFT = sf::Vector2f(1, 0);
static inline const auto BOW_RELOAD_HOLD_OFFSET_RIGHT = sf::Vector2f(-1, 0);

Bow::Bow() :
	Shooter(BOW_ID)
{
}

bool Bow::IsSilent() const
{
	return BOW_SILENT;
}

void Bow::Shoot(Direction facing)
{
	PlaySound(SOUND_FILENAME_SHOT);
	msp_Projectiles->emplace_back(std::make_unique<Arrow>(GetPos(), facing));
}

sf::Vector2f Bow::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return BOW_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return BOW_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return BOW_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return BOW_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

sf::Vector2f Bow::GetReloadHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return BOW_RELOAD_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return BOW_RELOAD_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return BOW_RELOAD_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return BOW_RELOAD_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Bow::GetHoldRotation(Direction facing) const
{
	switch (facing)
	{
	case Direction::DOWN:
		return BOW_HOLD_ROTATION_DOWN;
		break;
	case Direction::UP:
	case Direction::LEFT:
	case Direction::RIGHT:
		return NO_ROTATION;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Bow::GetReloadDelay() const
{
	return BOW_RELOAD_DELAY;
}

bool Bow::GetLoadedAmmoVisisble() const
{
	return BOW_AMMO_VISIBLE;
}

const std::string Bow::GetClassName() const
{
	return BOW_CLASS_NAME;
}