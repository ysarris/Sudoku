#include "Rocket.h"

static constexpr auto ROCKET_CLASS_NAME = "Rocket";
static constexpr auto ROCKET_DYING_TIMER = 0.25f;
static constexpr auto ROCKET_PRIM_SPEED = 250.0f;
static constexpr auto ROCKET_EXPLOSIVE = true;
static constexpr auto ROCKET_DAMAGE = 400;
static inline const auto ROCKET_SPAWN_OFFSET_UP = sf::Vector2f(0, -18); 
static inline const auto ROCKET_SPAWN_OFFSET_DOWN = sf::Vector2f(0, 20);
static inline const auto ROCKET_SPAWN_OFFSET_LEFT = sf::Vector2f(-24, -1);
static inline const auto ROCKET_SPAWN_OFFSET_RIGHT = sf::Vector2f(24, -1);

Rocket::Rocket(sf::Vector2f startPos, Direction facing) :
	Object(startPos),
	Projectile(startPos, ROCKET_DYING_TIMER, facing, ROCKET_PRIM_SPEED)
{
	SetSprite(ROCKET_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
}

void Rocket::HitWall(sf::Vector2f hitPos)
{
	StartDying(PROJECTILE_FILENAME_EXPLODE);
}

void Rocket::HitActor(Actor& actor)
{
	// Hasnt exploded yet ?
	if (GetStatus() == HealthStatus::ALIVE) { StartDying(PROJECTILE_FILENAME_EXPLODE); }
	// Has exploded and is dealing damage
	else { Projectile::HitActor(actor); }
}

sf::Vector2f Rocket::GetSpawnOffset() const
{
	switch (GetPrimaryMovement())
	{
	case Direction::UP:
		return ROCKET_SPAWN_OFFSET_UP;
		break;
	case Direction::DOWN:
		return ROCKET_SPAWN_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return ROCKET_SPAWN_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return ROCKET_SPAWN_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

bool Rocket::GetIsExplosive() const
{
	return ROCKET_EXPLOSIVE;
}

int Rocket::GetDamage() const
{
	return ROCKET_DAMAGE;
}

const std::string Rocket::GetClassName() const
{
	return ROCKET_CLASS_NAME;
}