#include "FireBottle.h"
#include "Flame.h"
#include "Random.h"			// for GetRandomDirection()
#include "Mathematics.h"	// for GetPointOnCircle()

static constexpr auto FIRE_BOTTLE_CLASS_NAME = "Fire Bottle";
// Weapon
static constexpr auto FIRE_BOTTLE_RELOAD_DELAY = 2.5f;
static constexpr auto FIRE_BOTTLE_HOLD_ROTATION_UP_LEFT = 30.0f;
static constexpr auto FIRE_BOTTLE_HOLD_ROTATION_DOWN_RIGHT = -30.0f;
static inline const auto FIRE_BOTTLE_HOLD_OFFSET_UP = sf::Vector2f(11, -4);
static inline const auto FIRE_BOTTLE_HOLD_OFFSET_DOWN = sf::Vector2f(-9, 0);
static inline const auto FIRE_BOTTLE_HOLD_OFFSET_LEFT = sf::Vector2f(7, -2);
static inline const auto FIRE_BOTTLE_HOLD_OFFSET_RIGHT = sf::Vector2f(-7, -2);
// Projectile
static constexpr auto FIRE_BOTTLE_FILENAME_SMASH = "Smash";
static constexpr auto FIRE_BOTTLE_MAX_TRAJECTORY_HEIGHT = 20.0f;
static constexpr auto FIRE_BOTTLE_DYING_TIMER = 0.01f;
static constexpr auto FIRE_BOTTLE_MAX_RANGE = 200.0f;
static constexpr auto FIRE_BOTTLE_DAMAGE = 10;
static constexpr auto FIRE_BOTTLE_DEGREES_ROTATION_RATE = 115;
static constexpr auto FIRE_BOTTLE_FLAMES_RELEASED = 6;
static constexpr auto FIRE_BOTTLE_FLAMES_DYING_TIMER = 3.0f;
static constexpr auto FIRE_BOTTLE_FLAMES_SPREAD_RADIUS = 20.0f;
static constexpr auto FIRE_BOTTLE_FLAMES_SPAWN_ANGLE_MULTIPLE = 360.0f / (FIRE_BOTTLE_FLAMES_RELEASED - 1);

// Weapon
FireBottle::FireBottle() :
	Throwable(FIRE_BOTTLE_ID)
{
}

sf::Vector2f FireBottle::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return FIRE_BOTTLE_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return FIRE_BOTTLE_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return FIRE_BOTTLE_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return FIRE_BOTTLE_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

// Projectile
FireBottle::FireBottle(sf::Vector2f startPos, Direction facing, float floorCoordinate, float startHeight) :
	Object(startPos),
	Throwable(startPos, FIRE_BOTTLE_DYING_TIMER, facing, floorCoordinate)
{
	SetSprite(FIRE_BOTTLE_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
	MakeTrajectoryParabolic(startHeight, FIRE_BOTTLE_MAX_TRAJECTORY_HEIGHT);
}

void FireBottle::HitWall(sf::Vector2f hitPos)
{
	Smash();
}

void FireBottle::HitFloor()
{
	Smash();
}

void FireBottle::HitActor(Actor& actor)
{
	Smash();
}

void FireBottle::Throw(Direction facing, float floorCoordinate, float startHeight)
{
	PlaySound(THROWABLE_THROW_FILENAME);
	msp_Projectiles->emplace_back(std::make_unique<FireBottle>(GetPos(), facing, floorCoordinate, startHeight));
}

float FireBottle::GetHoldRotation(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
	case Direction::LEFT:
		return FIRE_BOTTLE_HOLD_ROTATION_UP_LEFT;
		break;
	case Direction::DOWN:
	case Direction::RIGHT:
		return FIRE_BOTTLE_HOLD_ROTATION_DOWN_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float FireBottle::GetReloadDelay() const
{
	return FIRE_BOTTLE_RELOAD_DELAY;
}

void FireBottle::Smash()
{
	StartDying(FIRE_BOTTLE_FILENAME_SMASH);

	sf::Vector2f newPos;

	for (auto f = 0; f < FIRE_BOTTLE_FLAMES_RELEASED; f++)
	{
		// First flame goes in center and the rest around it
		newPos = (f == 0) ? GetPos() : GetPointOnCircle(GetPos(), FIRE_BOTTLE_FLAMES_SPREAD_RADIUS, f * FIRE_BOTTLE_FLAMES_SPAWN_ANGLE_MULTIPLE);
		msp_Projectiles->emplace_back(std::make_unique<Flame>(newPos, GetPrimaryMovement())); // use same direction for all flames so their spawn offsets are equal
		// Make flame start burning immediatelly
		dynamic_cast<Flame*>(msp_Projectiles->back().get())->Burn(FIRE_BOTTLE_FLAMES_DYING_TIMER);
	}
}

int FireBottle::GetDamage() const
{
	return FIRE_BOTTLE_DAMAGE;
}

float FireBottle::GetMaxRange() const
{
	return FIRE_BOTTLE_MAX_RANGE;
}

float FireBottle::GetRotationRate() const
{
	return FIRE_BOTTLE_DEGREES_ROTATION_RATE;
}

const std::string FireBottle::GetClassName() const
{
	return FIRE_BOTTLE_CLASS_NAME;
}