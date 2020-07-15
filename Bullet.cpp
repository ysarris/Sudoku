#include "Bullet.h"

static constexpr auto BULLET_CLASS_NAME = "Bullet";
static constexpr auto BULLET_DYING_TIMER = 0.1f;
static constexpr auto BULLET_PRIM_SPEED = 300.0f;
static constexpr auto BULLET_DAMAGE = 60;
static inline const auto BULLET_SPAWN_OFFSET_UP = sf::Vector2f(0, -1);
static inline const auto BULLET_SPAWN_OFFSET_DOWN = sf::Vector2f(0, 2);
static inline const auto BULLET_SPAWN_OFFSET_LEFT = sf::Vector2f(-3, -2);
static inline const auto BULLET_SPAWN_OFFSET_RIGHT = sf::Vector2f(3, -2);

Bullet::Bullet(sf::Vector2f startPos, Direction facing) :
	Object(startPos),
	Projectile(startPos, BULLET_DYING_TIMER, facing, BULLET_PRIM_SPEED)
{
	SetSprite(BULLET_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
}

const std::string Bullet::GetClassName() const
{
	return BULLET_CLASS_NAME;
}

sf::Vector2f Bullet::GetSpawnOffset() const
{
	switch (GetPrimaryMovement())
	{
	case Direction::UP:
		return BULLET_SPAWN_OFFSET_UP;
		break;
	case Direction::DOWN:
		return BULLET_SPAWN_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return BULLET_SPAWN_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return BULLET_SPAWN_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

int Bullet::GetDamage() const
{
	return BULLET_DAMAGE;
}