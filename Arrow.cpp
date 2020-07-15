#include "Arrow.h"
#include "Random.h"		// for GetRandomBool()

static constexpr auto ARROW_CLASS_NAME = "Arrow";
static constexpr auto ARROW_DYING_TIMER = 0.3f;
static constexpr auto ARROW_PRIM_SPEED = 400.0f;
static constexpr auto ARROW_DAMAGE = 50;
static inline const auto ARROW_SPAWN_OFFSET_UP = sf::Vector2f(0, -13);
static inline const auto ARROW_SPAWN_OFFSET_DOWN = sf::Vector2f(0, 5);
static inline const auto ARROW_SPAWN_OFFSET_LEFT = sf::Vector2f(-8, 0);
static inline const auto ARROW_SPAWN_OFFSET_RIGHT = sf::Vector2f(8, 0);

Arrow::Arrow(sf::Vector2f startPos, Direction facing) :
	Object(startPos),
	Projectile(startPos, ARROW_DYING_TIMER, facing, ARROW_PRIM_SPEED)
{
	SetSprite(ARROW_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
}

void Arrow::HitWall(sf::Vector2f hitPos)
{
	Projectile::HitWall(hitPos);
	// Make arrow look broken on a random side
	if (GetRandomBool()) { FlipSpriteVertically(); }
}

const std::string Arrow::GetClassName() const
{
	return ARROW_CLASS_NAME;
}

sf::Vector2f Arrow::GetSpawnOffset() const
{
	switch (GetPrimaryMovement())
	{
	case Direction::UP:
		return ARROW_SPAWN_OFFSET_UP;
		break;
	case Direction::DOWN:
		return ARROW_SPAWN_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return ARROW_SPAWN_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return ARROW_SPAWN_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

int Arrow::GetDamage() const
{
	return ARROW_DAMAGE;
}

float Arrow::GetDyingRotation() const
{
	// Keep same rotation as when arrow was spawned
	return GetSpawnRotation();
}