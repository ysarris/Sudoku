#include "Pistol.h"
#include "Bullet.h"

static constexpr auto PISTOL_CLASS_NAME = "Pistol";
static constexpr auto PISTOL_SHOT_DELAY = 0.3f;
static constexpr auto PISTOL_RELOAD_DELAY = 1.5f;
static constexpr auto PISTOL_CAPACITY = 12;
static inline const auto PISTOL_HOLD_OFFSET_UP = sf::Vector2f(0, -13);
static inline const auto PISTOL_HOLD_OFFSET_DOWN = sf::Vector2f(0, 5);
static inline const auto PISTOL_HOLD_OFFSET_LEFT = sf::Vector2f(-7, 5);
static inline const auto PISTOL_HOLD_OFFSET_RIGHT = sf::Vector2f(7, 5);

Pistol::Pistol() :
	Shooter(PISTOL_ID)
{
}

void Pistol::Shoot(Direction facing)
{
	PlaySound(SOUND_FILENAME_SHOT);
	msp_Projectiles->emplace_back(std::make_unique<Bullet>(GetPos(), facing));
}

sf::Vector2f Pistol::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return PISTOL_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return PISTOL_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return PISTOL_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return PISTOL_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Pistol::GetShotDelay() const
{
	return PISTOL_SHOT_DELAY;
}

float Pistol::GetReloadDelay() const
{
	return PISTOL_RELOAD_DELAY;
}

int Pistol::GetCapacity() const
{
	return PISTOL_CAPACITY;
}

const std::string Pistol::GetClassName() const
{
	return PISTOL_CLASS_NAME;
}