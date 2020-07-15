#include "Grenade.h"

static constexpr auto GRENADE_CLASS_NAME = "Grenade";
// Weapon
static constexpr auto GRENADE_RELOAD_DELAY = 1.5f;
static inline const auto GRENADE_HOLD_OFFSET_UP = sf::Vector2f(10, -1);
static inline const auto GRENADE_HOLD_OFFSET_DOWN = sf::Vector2f(-7, 1);
static inline const auto GRENADE_HOLD_OFFSET_LEFT = sf::Vector2f(2, 0);
static inline const auto GRENADE_HOLD_OFFSET_RIGHT = sf::Vector2f(-2, 0);
// Projectile
static constexpr auto GRENADE_MAX_TRAJECTORY_HEIGHT = 25.0f;
static constexpr auto GRENADE_DETONATION_TIME = 2.5f;
static constexpr auto GRENADE_ENEMY_HIT_STUN_TIME = 0.4f;
static constexpr auto GRENADE_ENEMY_HIT_SPEED_REMAINING = 0.4f;
static constexpr auto GRENADE_DYING_TIMER = 0.2f;
static constexpr auto GRENADE_MAX_RANGE = 230.0f;
static constexpr auto GRENADE_EXPLOSIVE = true;
static constexpr auto GRENADE_DAMAGE = 250;
static constexpr auto GRENADE_DEGREES_ROTATION_RATE = 700;

// Weapon
Grenade::Grenade() :
	Throwable(GRENADE_ID),
	m_DetonationTimer()
{
}

void Grenade::Throw(Direction facing, float floorCoordinate, float startHeight)
{
	PlaySound(THROWABLE_THROW_FILENAME);
	msp_Projectiles->emplace_back(std::make_unique<Grenade>(GetPos(), facing, floorCoordinate, startHeight));
}

sf::Vector2f Grenade::GetHoldOffset(Direction facing) const
{
	switch (facing)
	{
	case Direction::UP:
		return GRENADE_HOLD_OFFSET_UP;
		break;
	case Direction::DOWN:
		return GRENADE_HOLD_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return GRENADE_HOLD_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return GRENADE_HOLD_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Grenade::GetReloadDelay() const
{
	return GRENADE_RELOAD_DELAY;
}

// Projectile
Grenade::Grenade(sf::Vector2f startPos, Direction facing, float floorCoordinate, float startHeight) :
	Object(startPos),
	Throwable(startPos, GRENADE_DYING_TIMER, facing, floorCoordinate),
	m_DetonationTimer(GRENADE_DETONATION_TIME)
{
	SetSprite(GRENADE_CLASS_NAME);
	if (facing == Direction::UP) { FlipSpriteHorizontally(); }	// Flip sprite so safety lever is facing same way as weapon
	AdjustSpawnPositionAndRotation();
	MakeTrajectoryParabolic(startHeight, GRENADE_MAX_TRAJECTORY_HEIGHT);
}

void Grenade::HitWall(sf::Vector2f hitPos)
{
	Throwable::HitWall(hitPos);
	WallBounce();
}

void Grenade::HitFloor()
{
	Throwable::HitFloor();
	FloorBounce();
}

void Grenade::HitActor(Actor& actor)
{
	// Hasnt exploded yet ?
	if (GetStatus() == HealthStatus::ALIVE)
	{
		PlaySound(PROJECTILE_FILENAME_HIT_ACTOR);
		m_HitTimer.Reset(GetHitDelay());

		// Bounce off actor with new primary speed and stun them
		TurnOppositeDirPrim();
		SetPrimarySpeed(abs(GetPrimarySpeed()) * GRENADE_ENEMY_HIT_SPEED_REMAINING);
		actor.StunFor(GRENADE_ENEMY_HIT_STUN_TIME);

		// New floor coordinate at actor feet
		SetFloorCoordinate(actor.GetFeetCoordinate());
	}
	else { Projectile::HitActor(actor); }
}

bool Grenade::GetIsExplosive() const
{
	return GRENADE_EXPLOSIVE;
}

int Grenade::GetDamage() const
{
	return GRENADE_DAMAGE;
}

float Grenade::GetMaxRange() const
{
	return GRENADE_MAX_RANGE;
}

float Grenade::GetRotationRate() const
{
	return GRENADE_DEGREES_ROTATION_RATE;
}

void Grenade::Update(float dtAsSeconds)
{
	// Default throwable updates
	Throwable::Update(dtAsSeconds);

	// Grenade projectile updates
	if (!IsWeapon())
	{
		// Hasnt exploded yet ?
		if (GetStatus() == HealthStatus::ALIVE)
		{
			// When grenade resting on floor dont check for enemy collision
			// by constantly reseting the last hit timer
			if (GetPrimaryMovement() == Direction::NONE) { m_HitTimer.Reset(GetHitDelay()); }

			// Time to explode ?
			if (m_DetonationTimer.RanOut()) { StartDying(PROJECTILE_FILENAME_EXPLODE); }
			else { m_DetonationTimer.Decrement(dtAsSeconds); }
		}
	}
}

const std::string Grenade::GetClassName() const
{
	return GRENADE_CLASS_NAME;
}