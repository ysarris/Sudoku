#include "Projectile.h"
#include "Mathematics.h"	// for CalcDistOneAxis()
#include "Random.h"			// for GetRandomBool() and GetRandomAngleOfMultiple()

static constexpr auto PROJECTILE_CLASS_NAME = "Projectile";
static constexpr auto PROJECTILE_HIT_DELAY = 0.15f;
static constexpr auto PROJECTILE_DEFAULT_MAX_RANGE = 600.0f;
static constexpr auto PROJECTILE_DEFAULT_EXPLOSIVE = false;
static constexpr auto PROJECTILE_DEFAULT_SPAWN_ROTATION_UP = -90.0f;
static constexpr auto PROJECTILE_DEFAULT_SPAWN_ROTATION_DOWN = 90.0f;
static constexpr auto PROJECTILE_DEFAULT_SPAWN_ROTATION_LEFT = 180.0f;
static constexpr auto PROJECTILE_DEFAULT_DYING_ROTATION_MULTIPLE = 90.0f;
static constexpr auto PROJECTILE_ATTACH_ADJUSTMENT_MULTIPLE = 1.3f;
static constexpr auto ERROR_PROJECTILE_DYING_TIMER_INVALID = "Projectile cant start with a negative or zero dying timer - value must be > 0.";
static constexpr auto ERROR_PROJECTILE_PRIM_MOV_NONE = "Projectile cant have no starting primary movement.";
static constexpr auto ERROR_PROJECTILE_PRIM_SPEED_GREATER_THAN_LIMIT = "Projectile cant have a primary speed greater than the limit.";
static constexpr auto ERROR_PROJECTILE_SPREAD_MIN_SPEED_NEGATIVE = "Min primary speed for a spread trajectory must not be negative.";
static constexpr auto ERROR_PROJECTILE_SPREAD_MIN_SPEED_GREATER_THAN_MAX = "Min primary speed for a spread trajectory must not be greater than the max.";
static constexpr auto ERROR_PROJECTILE_SPREAD_MIN_SPEED_TOO_SLOW = "Min primary speed for a spread trajectory must be greater than the max throw speed, otherwise trajectory would be parabolic.";

float Projectile::GetDistanceLeftToTravel() const
{
	Assert(ERROR_PROJECTILE_MAX_RANGE_NEGATIVE, GetMaxRange() >= 0);
	return GetMaxRange() - m_DistanceTravelled;
}

bool Projectile::IsExploding() const
{
	return GetStatus() == HealthStatus::DYING && GetIsExplosive();
}

bool Projectile::IsReadyForWallCollision() const
{
	return GetStatus() == HealthStatus::ALIVE && m_WallHitTimer.RanOut() && GetPrimaryMovement() != Direction::NONE;
}

bool Projectile::IsReadyForCollision() const
{
	return Movable::IsReadyForCollision() || (IsExploding() && GetDyingTimeLeft() > DEFAULT_TIMER_VALUE);
}

void Projectile::HitActor(Actor& actor)
{
	actor.HitByProjectile(GetDamage(), IsExploding());

	if (GetStatus() == HealthStatus::ALIVE)
	{
		StartDying(PROJECTILE_FILENAME_HIT_ACTOR);
	
		// Can it attach to actor ? (e.g. arrow, bullet blood splat)
		if (ImageExists(PROJECTILE_FILENAME_HIT_ACTOR))
		{
			// Projectiles shot/thrown from very close range can overshoot actor and appear stuck in the air
			// Calculate the edges of the projectile and actor depending on primary movement of projectile
			auto isVertical = IsDirectionVertical(GetPrimaryMovement());
			auto negate = GetPrimaryMovement() == (isVertical ? Direction::UP : Direction::LEFT);
			auto edgeProj = (isVertical ? GetPos().y : GetPos().x) + (negate ? -1 : 1) * HalfOf(isVertical ? GetHeight() : GetWidth());
			auto edgeActor = (isVertical ? actor.GetPos().y : actor.GetPos().x) + (negate ? -1 : 1) * HalfOf(isVertical ? actor.GetHeight() : actor.GetWidth());

			// Projectile overshot actor ?
			if ((negate ? edgeProj : edgeActor) < (negate ? edgeActor : edgeProj))
			{
				// Add or subtract adjustment ?
				auto adjustment = (negate ? 1 : -1) * PROJECTILE_ATTACH_ADJUSTMENT_MULTIPLE * CalcDistOneAxis(edgeProj, edgeActor);
				// Adjust y or x coordinate ?
				SetPos(sf::Vector2f(GetPos().x + (isVertical ? 0 : 1) * adjustment, GetPos().y + (isVertical ? 1 : 0) * adjustment));
			}

			// Attach to actor
			mp_Attach = std::make_unique<AttachedToActor>(&actor, false, GetPos() - actor.GetPos());
		}
	}
}

void Projectile::HitWall(sf::Vector2f hitPos)
{
	StartDying(PROJECTILE_FILENAME_HIT_WALL);
	m_WallHitTimer.Reset(GetHitDelay());
}

void Projectile::Update(float dtAsSeconds)
{
	// Default movable updates
	Movable::Update(dtAsSeconds);

	// Projectile updates
	if (GetStatus() == HealthStatus::ALIVE)
	{
		Move(dtAsSeconds);
		if (!m_WallHitTimer.RanOut()) { m_WallHitTimer.Decrement(dtAsSeconds); }
	}
	else
	{
		// Attached to an actor ?
		if (mp_Attach != nullptr)
		{
			// Actor not dead yet ?
			if (mp_Attach->pActor->GetStatus() != HealthStatus::DEAD)
			{
				// Keep projectile in dying state until actor has finished dying
				SetDyingTimer(mp_Attach->pActor->GetDyingTimeLeft());

				// Move with alive actor
				if (mp_Attach->pActor->GetStatus() == HealthStatus::ALIVE) { SetPos(mp_Attach->pActor->GetPos() + mp_Attach->offset); }
				// Set final position and rotation when actor starts dying
				else if (mp_Attach->pActor->GetStatus() == HealthStatus::DYING && !mp_Attach->startedDying)
				{
					mp_Attach->startedDying = true;

					// Rotate projectile with dying actor
					auto actorRotation = mp_Attach->pActor->GetRotation();
					RotateSpriteBy(actorRotation);
					// Determine which side actor is on rotated onto
					auto onDefaultSide = actorRotation == ACTOR_DEFAULT_DYING_ROTATION;
			
					// Dying actor is on their side so reverse the offsets,
					// adjust x coordinate with y-offset and y coordinate with x-offset
					SetPosX(mp_Attach->pActor->GetPos().x + (onDefaultSide ? -1 : 1) * mp_Attach->offset.y);
					SetPosY(mp_Attach->pActor->GetPos().y + (onDefaultSide ? 1 : -1) * mp_Attach->offset.x);
				}
			}
			// Detach from dead actor
			else { mp_Attach.reset(); }
		}
	}
}

void Projectile::Draw(sf::RenderWindow& rWindow) const
{
	// Only draw projectiles whose dying timer has not ran out yet, this prevents drawing projectiles
	// that are technically still dying due to their dying sound still playing (e.g. exploding grenade)
	if (GetDyingTimeLeft() > DEFAULT_TIMER_VALUE) { Object::Draw(rWindow); }
}

Projectile::Projectile(sf::Vector2f startPos, float dyingTimer, Direction primMov, float primSpeed) :
	Object(startPos),
	Movable(startPos, dyingTimer, primMov, primSpeed, DEFAULT_DIRECTION, DEFAULT_SPEED),
	m_DistanceTravelled(),
	m_WallHitTimer()
{
	AddToDirectory(PROJECTILE_CLASS_NAME);
	Assert(ERROR_PROJECTILE_DYING_TIMER_INVALID, dyingTimer > DEFAULT_TIMER_VALUE);
	Assert(ERROR_PROJECTILE_PRIM_MOV_NONE, primMov != Direction::NONE);
	Assert(ERROR_PROJECTILE_PRIM_SPEED_GREATER_THAN_LIMIT, primSpeed <= PROJECTILE_MAX_SPEED);
}

Projectile::Projectile() :
	Movable(DEFAULT_POSITION, DEFAULT_TIMER_VALUE, DEFAULT_DIRECTION, DEFAULT_SPEED, DEFAULT_DIRECTION, DEFAULT_SPEED),
	m_DistanceTravelled(),
	m_WallHitTimer()
{
	// Doesnt add "Projectile/" to throwable weapon directory
	// Doesnt check primary movement and primary speed of throwable weapon instance
}

float Projectile::GetDistanceTravelled() const
{
	return m_DistanceTravelled;
}

void Projectile::AdjustSpawnPositionAndRotation()
{
	SetPos(GetPos() + GetSpawnOffset());
	RotateSpriteBy(GetSpawnRotation());
}

void Projectile::MakeTrajectorySpread(float minPrimarySpeed, float maxPrimarySpeed, float maxAbsoluteSecondarySpeed)
{
	Assert(ERROR_PROJECTILE_SPREAD_MIN_SPEED_NEGATIVE, minPrimarySpeed > 0);
	Assert(ERROR_PROJECTILE_SPREAD_MIN_SPEED_GREATER_THAN_MAX, minPrimarySpeed < maxPrimarySpeed);
	Assert(ERROR_PROJECTILE_PRIM_SPEED_GREATER_THAN_LIMIT, maxPrimarySpeed <= PROJECTILE_MAX_SPEED);
	Assert(ERROR_PROJECTILE_SPREAD_MIN_SPEED_TOO_SLOW, minPrimarySpeed >= THROWABLE_MAX_SPEED);

	if (IsDirectionHorizontal(GetPrimaryMovement())) { SetSecondaryMovement(GetRandomBool() ? Direction::DOWN : Direction::UP); }
	else if (IsDirectionVertical(GetPrimaryMovement())) { SetSecondaryMovement(GetRandomBool() ? Direction::RIGHT : Direction::LEFT); }

	SetPrimarySpeed(static_cast<float>(GetRandomNumberBetween(static_cast<int>(minPrimarySpeed), static_cast<int>(maxPrimarySpeed))));
	SetSecondarySpeed(static_cast<float>(GetRandomNumberBetween(static_cast<int>(DEFAULT_SPEED), static_cast<int>(maxAbsoluteSecondarySpeed))));
}

sf::Vector2f Projectile::GetSpawnOffset() const
{
	return NO_OFFSET;
}

float Projectile::GetSpawnRotation() const
{
	switch (GetPrimaryMovement())
	{
	case Direction::UP:
		return PROJECTILE_DEFAULT_SPAWN_ROTATION_UP;
		break;
	case Direction::DOWN:
		return PROJECTILE_DEFAULT_SPAWN_ROTATION_DOWN;
		break;
	case Direction::LEFT:
		return PROJECTILE_DEFAULT_SPAWN_ROTATION_LEFT;
		break;
	case Direction::RIGHT:
		return NO_ROTATION;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Projectile::GetHitDelay() const
{
	return PROJECTILE_HIT_DELAY;
}

float Projectile::GetMaxRange() const
{
	return PROJECTILE_DEFAULT_MAX_RANGE;
}

void Projectile::UpdateDistanceTravelled(sf::Vector2f oldPos)
{
	if (IsDirectionHorizontal(GetPrimaryMovement())) { m_DistanceTravelled += CalcDistOneAxis(oldPos.x, GetPos().x); }
	else if (IsDirectionVertical(GetPrimaryMovement())) { m_DistanceTravelled += CalcDistOneAxis(oldPos.y, GetPos().y); }

	Assert(ERROR_PROJECTILE_MAX_RANGE_NEGATIVE, GetMaxRange() >= 0);
	if (m_DistanceTravelled >= GetMaxRange()) { ReachedMaxDistance(); }
}

void Projectile::Move(float dtAsSeconds)
{
	if (GetPrimaryMovement() != Direction::NONE)
	{
		auto oldPos = GetPos();
		Movable::Move(dtAsSeconds);
		UpdateDistanceTravelled(oldPos);
		RotateSpriteBy(dtAsSeconds * (GetPrimaryMovement() != Direction::RIGHT ? -1 : 1) * GetRotationRate());
	}
}

void Projectile::ReachedMaxDistance()
{
	SetHealthStatus(HealthStatus::DEAD);
}

bool Projectile::GetIsExplosive() const
{
	return PROJECTILE_DEFAULT_EXPLOSIVE;
}

float Projectile::GetRotationRate() const
{
	return NO_ROTATION;
}

float Projectile::GetDyingRotation() const
{
	return GetRandomAngleOfMultiple(PROJECTILE_DEFAULT_DYING_ROTATION_MULTIPLE);
}