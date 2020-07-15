#include "Shotgun.h"
#include "Pellet.h"

static constexpr auto SHOTGUN_CLASS_NAME = "Shotgun";
static constexpr auto SHOTGUN_SHOT_DELAY = 0.4f;
static constexpr auto SHOTGUN_RELOAD_DELAY = 1.3f;
static constexpr auto SHOTGUN_CAPACITY = 2;
static constexpr auto SHOTGUN_PELLETS_PER_SHOT = 10;
static inline const auto SHOTGUN_HOLD_OFFSET_UP = sf::Vector2f(0, -15);
static inline const auto SHOTGUN_HOLD_OFFSET_DOWN = sf::Vector2f(0, 7);
static inline const auto SHOTGUN_HOLD_OFFSET_LEFT = sf::Vector2f(-5, 4);
static inline const auto SHOTGUN_HOLD_OFFSET_RIGHT = sf::Vector2f(5, 4);

Shotgun::Shotgun() :
	Shooter(SHOTGUN_ID)
{
}

void Shotgun::Shoot(Direction facing)
{
	PlaySound(SOUND_FILENAME_SHOT);
	for (auto p = 0; p < SHOTGUN_PELLETS_PER_SHOT; p++)
		msp_Projectiles->emplace_back(std::make_unique<Pellet>(GetPos(), facing));
}

sf::Vector2f Shotgun::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return SHOTGUN_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return SHOTGUN_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return SHOTGUN_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return SHOTGUN_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Shotgun::GetShotDelay() const
{
	return SHOTGUN_SHOT_DELAY;
}

float Shotgun::GetReloadDelay() const
{
	return SHOTGUN_RELOAD_DELAY;
}

int Shotgun::GetCapacity() const
{
	return SHOTGUN_CAPACITY;
}

const std::string Shotgun::GetClassName() const
{
	return SHOTGUN_CLASS_NAME;
}