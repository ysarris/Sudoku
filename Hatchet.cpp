#include "Hatchet.h"

static constexpr auto HATCHET_CLASS_NAME = "Hatchet";
// Weapon
static constexpr auto HATCHET_RELOAD_DELAY = 2.0f;
static constexpr auto HATCHET_HOLD_ROTATION_UP_LEFT = 45.0f;
static constexpr auto HATCHET_HOLD_ROTATION_DOWN_RIGHT = -45.0f;
static inline const auto HATCHET_HOLD_OFFSET_UP = sf::Vector2f(10, 2);
static inline const auto HATCHET_HOLD_OFFSET_DOWN = sf::Vector2f(-10, 2);
static inline const auto HATCHET_HOLD_OFFSET_LEFT = sf::Vector2f(3, 0);
static inline const auto HATCHET_HOLD_OFFSET_RIGHT = sf::Vector2f(-3, 0);
// Projectile
static constexpr auto HATCHET_FILENAME_IN_FLOOR = "InFloor";
static constexpr auto HATCHET_FILENAME_ON_FLOOR = "OnFloor";
static constexpr auto HATCHET_ON_FLOOR_MIN_ROTATION_TO_FLIP = 180.0f;
static constexpr auto HATCHET_MAX_TRAJECTORY_HEIGHT = 15.0f;
static constexpr auto HATCHET_DYING_TIMER = 2.7f;
static constexpr auto HATCHET_MAX_RANGE = 155.0f;
static constexpr auto HATCHET_DAMAGE = 150;
static constexpr auto HATCHET_ROTATION_RATE = 800;
static constexpr auto HATCHET_IN_ACTOR_DYING_ROTATION_UP_DOWN = 155.0f;
static constexpr auto HATCHET_IN_ACTOR_DYING_ROTATION_LEFT = 65.0f;
static constexpr auto HATCHET_IN_ACTOR_DYING_ROTATION_RIGHT = -65.0f;
static constexpr auto HATCHET_IN_FLOOR_DYING_ROTATION_UP = 70.0f;
static constexpr auto HATCHET_IN_FLOOR_DYING_ROTATION_DOWN = -110.0f;
static constexpr auto HATCHET_IN_FLOOR_DYING_ROTATION_LEFT = -20.0f;
static constexpr auto HATCHET_IN_FLOOR_DYING_ROTATION_RIGHT = 20.0f;

// Weapon
Hatchet::Hatchet() :
	Throwable(HATCHET_ID)
{
}

sf::Vector2f Hatchet::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return HATCHET_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return HATCHET_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return HATCHET_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return HATCHET_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

// Projectile
Hatchet::Hatchet(sf::Vector2f startPos, Direction facing, float floorCoordinate, float startHeight) :
	Object(startPos),
	Throwable(startPos, HATCHET_DYING_TIMER, facing, floorCoordinate)
{
	SetSprite(HATCHET_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
	MakeTrajectoryParabolic(startHeight, HATCHET_MAX_TRAJECTORY_HEIGHT);
}

void Hatchet::HitFloor()
{
	if (HasHitWall())
	{
		auto oldRotation = GetRotation();
		StartDying(HATCHET_FILENAME_ON_FLOOR);
		// Make it drop on correct side
		if (oldRotation >= HATCHET_ON_FLOOR_MIN_ROTATION_TO_FLIP) { FlipSpriteHorizontally(); }
	}
	else
	{
		StartDying(HATCHET_FILENAME_IN_FLOOR);
		// Make it look stuck in floor
		if (GetPrimaryMovement() != Direction::RIGHT) { FlipSpriteHorizontally(); }
	}
}

void Hatchet::HitWall(sf::Vector2f hitPos)
{
	Throwable::HitWall(hitPos);
	WallDrop();
}

void Hatchet::HitActor(Actor& actor)
{
	// Cant damage actor after bouncing off wall
	if (!HasHitWall())
	{
		// Checked in HasHitActor(), needs to be set before hatchet starts dying for correct rotation
		SetPrimarySpeed(DEFAULT_SPEED);

		// Default projectile hit
		Projectile::HitActor(actor);

		// Make it look stuck in actor
		if (GetPrimaryMovement() == Direction::DOWN) { FlipSpriteVertically(); }
		else if (GetPrimaryMovement() == Direction::LEFT || GetPrimaryMovement() == Direction::UP) { FlipSpriteHorizontally(); }
	}
}

void Hatchet::Throw(Direction facing, float floorCoordinate, float startHeight)
{
	PlaySound(THROWABLE_THROW_FILENAME);
	msp_Projectiles->emplace_back(std::make_unique<Hatchet>(GetPos(), facing, floorCoordinate, startHeight));
}

float Hatchet::GetHoldRotation(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
	case Direction::LEFT:
		return HATCHET_HOLD_ROTATION_UP_LEFT;
		break;
	case Direction::DOWN:
	case Direction::RIGHT:
		return HATCHET_HOLD_ROTATION_DOWN_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Hatchet::GetReloadDelay() const
{
	return HATCHET_RELOAD_DELAY;
}

bool Hatchet::HasHitWall() const
{
	return abs(GetPrimarySpeed()) > THROWABLE_HIT_WALL_DROP_SPEED ? false : true;
}

bool Hatchet::HasHitActor() const
{
	return abs(GetPrimarySpeed()) == DEFAULT_SPEED ? true : false;
}

int Hatchet::GetDamage() const
{
	return HATCHET_DAMAGE;
}

float Hatchet::GetMaxRange() const
{
	return HATCHET_MAX_RANGE;
}

float Hatchet::GetRotationRate() const
{
	return HasHitWall() ? NO_ROTATION : HATCHET_ROTATION_RATE;
}

float Hatchet::GetDyingRotation() const
{
	// Stuck in enemy ?
	if (HasHitActor())
	{
		switch (GetPrimaryMovement())
		{
		case Direction::UP:
		case Direction::DOWN:
			return HATCHET_IN_ACTOR_DYING_ROTATION_UP_DOWN;
			break;
		case Direction::LEFT:
			return HATCHET_IN_ACTOR_DYING_ROTATION_LEFT;
			break;
		case Direction::RIGHT:
			return HATCHET_IN_ACTOR_DYING_ROTATION_RIGHT;
			break;
		default:
			Assert(ERROR_UNKNOWN_DIRECTION);
			break;
		}
	}
	// Dropped on floor ?
	else if (HasHitWall())
		return NO_ROTATION;
	// Stuck in floor
	else
	{
		switch (GetPrimaryMovement())
		{
		case Direction::UP:
			return HATCHET_IN_FLOOR_DYING_ROTATION_UP;
			break;
		case Direction::DOWN:
			return HATCHET_IN_FLOOR_DYING_ROTATION_DOWN;
			break;
		case Direction::LEFT:
			return HATCHET_IN_FLOOR_DYING_ROTATION_LEFT;
			break;
		case Direction::RIGHT:
			return HATCHET_IN_FLOOR_DYING_ROTATION_RIGHT;
			break;
		default:
			Assert(ERROR_UNKNOWN_DIRECTION);
			break;
		}
	}
}

const std::string Hatchet::GetClassName() const
{
	return HATCHET_CLASS_NAME;
}