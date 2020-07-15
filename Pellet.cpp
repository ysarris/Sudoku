#include "Pellet.h"

static constexpr auto PELLET_CLASS_NAME = "Pellet";
static constexpr auto PELLET_DYING_TIMER = 0.15f;
static constexpr auto PELLET_MIN_PRIM_SPEED = 250.0f;
static constexpr auto PELLET_MAX_PRIM_SPEED = 350.0f;
static constexpr auto PELLET_MAX_ABS_SEC_SPEED = 50.0f;
static constexpr auto PELLET_MAX_RANGE = 180.0f;
static constexpr auto PELLET_DAMAGE = 35;
static inline const auto PELLET_SPAWN_OFFSET_UP = sf::Vector2f(-1, -4);
static inline const auto PELLET_SPAWN_OFFSET_DOWN = sf::Vector2f(-1, 4);
static inline const auto PELLET_SPAWN_OFFSET_LEFT = sf::Vector2f(-10, -2);
static inline const auto PELLET_SPAWN_OFFSET_RIGHT = sf::Vector2f(10, -2);

Pellet::Pellet(sf::Vector2f startPos, Direction facing) :
	Object(startPos),
	Projectile(startPos, PELLET_DYING_TIMER, facing)
{
	SetSprite(PELLET_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
	MakeTrajectorySpread(PELLET_MIN_PRIM_SPEED, PELLET_MAX_PRIM_SPEED, PELLET_MAX_ABS_SEC_SPEED);
}

const std::string Pellet::GetClassName() const
{
	return PELLET_CLASS_NAME;
}

sf::Vector2f Pellet::GetSpawnOffset() const
{
	switch (GetPrimaryMovement())
	{
	case Direction::UP:
		return PELLET_SPAWN_OFFSET_UP;
		break;
	case Direction::DOWN:
		return PELLET_SPAWN_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return PELLET_SPAWN_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return PELLET_SPAWN_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Pellet::GetMaxRange() const
{
	return PELLET_MAX_RANGE;
}

int Pellet::GetDamage() const
{
	return PELLET_DAMAGE;
}