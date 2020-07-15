#include "Throwable.h"
#include "Random.h"			// for GetRandomBool() and GetRandomNumberBetween()
#include "Mathematics.h"	// for HalfOf(), CalcDistOneAxis and GetPositiveQuadraticSolution()
#include "Walls_info.h"		// for wall boundaries

static constexpr auto THROWABLE_CLASS_NAME = "Throwable";
static constexpr auto THROWABLE_SILENT = true;
static constexpr auto THROWABLE_AMMO_VISIBLE = true;
static constexpr auto THROWABLE_HIT_WALL_DROP_HORIZONTAL_SPEED = 20.0f;	// used as primary speed for horizontal primary movement
static constexpr auto THROWABLE_HIT_WALL_DROP_SEC_MOVEMENT = Direction::DOWN;
static constexpr auto THROWABLE_HIT_WALL_DROP_DIST_MIN = 3;
static constexpr auto THROWABLE_HIT_WALL_DROP_DIST_MAX = 5;
static constexpr auto THROWABLE_HIT_WALL_BOUNCE_SPEED_REMAINING = 0.5f;
static constexpr auto THROWABLE_HIT_FLOOR_DEPTH_OFFSET = 2.0f; // gives a little depth to throwables hitting floor
static constexpr auto THROWABLE_HIT_FLOOR_BOUNCE_SPEED_REMAINING = 0.35f;
static constexpr auto THROWABLE_HIT_FLOOR_ROLL_SPEED_REMAINING = 0.7f;
static constexpr auto THROWABLE_HIT_FLOOR_ROLL_SPEED_MIN = 50.0f;
static constexpr auto THROWABLE_HIT_FLOOR_ROLL_SPEED_MAX = 60.0f;
static constexpr auto ERROR_THROWABLE_WALL_DROP = "Only call WallDrop() function right after hitting a wall.";
static constexpr auto ERROR_THROWABLE_WALL_BOUNCE = "Only call WallBounce() function right after hitting a wall.";
static constexpr auto ERROR_THROWABLE_FLOOR_BOUNCE = "Only call FloorBounce() function right after hitting the floor.";
static constexpr auto ERROR_THROWABLE_INITIAL_HEIGHT_GREATER_THAN_MAX = "Throwable initial height cant be greater than max height of trajectory";
static constexpr auto ERROR_THROWABLE_MAX_HEIGHT_GREATER_THAN_LIMIT = "Throwable max height cant be greater than trajectory height limit";
static constexpr auto ERROR_THROWABLE_TOO_FAST_FOR_PARABOLIC = "Throwable horizontal speed too fast for parabolic trajectory. To reduce speed increase max height and/or decrease range.";
static constexpr auto GRAVITY_ACCELERATION = 155.6f; // Gravity scaled for game world
													 // - Player height = 27 pixels
													 // - Assume in meters = 1.70m -> 1 pixel = 0.063m
													 // - Gravity = 9.807m/s^2 -> 9.807 / 0.063 = 155.6p/s^2

// For horizontal movement returns the height above floor coordinate
// For vertical movement returns the horizontal distance from floor coordinate
float Throwable::GetDistanceFromFloor() const
{
	auto bottomOfThrowable = GetPos().y + GetBottomAdjustment();
	auto heightAboveFloor = GetFloorCoordinate() - bottomOfThrowable;

	// Throwables moving UP have a greater Y coordinate than their floor coordinate, so reverse sign
	// (do NOT return absolute value as negative value indicates when the floor has been hit)
	return GetPrimaryMovement() == Direction::UP ? -heightAboveFloor : heightAboveFloor;
}

bool Throwable::IsReadyForFloorCollision() const
{
	return GetPrimaryMovement() != DEFAULT_DIRECTION && GetStatus() == HealthStatus::ALIVE && m_FloorHitTimer.RanOut();
}

void Throwable::HitFloor()
{
	PlaySound(THROWABLE_HIT_FLOOR_FILENAME);
	m_FloorHitTimer.Reset(GetHitDelay());
}

void Throwable::HitWall(sf::Vector2f hitPos)
{
	PlaySound(PROJECTILE_FILENAME_HIT_WALL);
	m_WallHitTimer.Reset(GetHitDelay());
	TurnOppositeDirPrim();
}

// Weapon instance of a throwable weapon
Throwable::Throwable(int weaponID) :
	Weapon(weaponID),
	m_FloorHitTimer(),
	m_FloorCoordinate()
{
	AddToDirectory(THROWABLE_CLASS_NAME);
}

bool Throwable::IsSilent() const
{
	return THROWABLE_SILENT;
}

void Throwable::Pause()
{
	IsWeapon() ? Weapon::Pause() : Projectile::Pause();
}

void Throwable::Resume()
{
	IsWeapon() ? Weapon::Resume() : Projectile::Resume();
}

void Throwable::Update(float dtAsSeconds)
{
	if (IsWeapon())
		// Default weapon updates
		Weapon::Update(dtAsSeconds);
	else
	{
		// Default projectile updates
		Projectile::Update(dtAsSeconds);

		// Throwable projectile updates
		if (GetStatus() == HealthStatus::ALIVE && GetPrimaryMovement() != DEFAULT_DIRECTION)
		{
			ApplyGravity(dtAsSeconds);
			if (!m_FloorHitTimer.RanOut()) { m_FloorHitTimer.Decrement(dtAsSeconds); }
		}
	}
}

void Throwable::Draw(sf::RenderWindow& rWindow) const
{
	if (IsWeapon()) { if (IsLoaded()) { Weapon::Draw(rWindow); } }
	else { Projectile::Draw(rWindow); }
}

// Distinguish weapon instance from projectile instance of a throwable
bool Throwable::IsWeapon() const
{
	return GetID() != WEAPON_DEFAULT_ID;
}

// Projectile instance of a throwable weapon (e.g. when a grenade or hatchet is thrown by player)
Throwable::Throwable(sf::Vector2f startPos, float dyingTimer, Direction primMov, float floorCoordinate) :
	Projectile(startPos, dyingTimer, primMov),
	Weapon(WEAPON_DEFAULT_ID), // projectiles dont have a weapon ID so use the default ID
	m_FloorHitTimer(),
	m_FloorCoordinate(floorCoordinate)
{
	AddToDirectory(THROWABLE_CLASS_NAME);
}

float Throwable::GetFloorCoordinate() const
{
	return m_FloorCoordinate;
}

void Throwable::WallDrop()
{
	Assert(ERROR_THROWABLE_WALL_DROP, !m_WallHitTimer.RanOut());

	if (IsDirectionHorizontal(GetPrimaryMovement()))
	{
		// Hit causes significant primary speed loss
		SetPrimarySpeed(THROWABLE_HIT_WALL_DROP_HORIZONTAL_SPEED);

		// Drop to floor using secondary speed
		SetSecondaryMovement(THROWABLE_HIT_WALL_DROP_SEC_MOVEMENT);
		SetSecondarySpeed(THROWABLE_HIT_WALL_DROP_SPEED);
	}
	else if (IsDirectionVertical(GetPrimaryMovement()))
	{
		// Drop to floor using primary speed
		SetPrimarySpeed(THROWABLE_HIT_WALL_DROP_SPEED);

		// Update floor coordinate
		auto newFloor = GetPrimaryMovement() == Direction::UP ? BOTTOM_WALL : TOP_WALL; // primary movement flipped in HitWall()
		auto distanceFromWall = GetRandomNumberBetween(THROWABLE_HIT_WALL_DROP_DIST_MIN, THROWABLE_HIT_WALL_DROP_DIST_MAX);
		SetFloorCoordinate(newFloor + distanceFromWall);
	}
}

void Throwable::WallBounce()
{
	Assert(ERROR_THROWABLE_WALL_BOUNCE, !m_WallHitTimer.RanOut());

	// Bounce causes some primary speed loss
	SetPrimarySpeed(abs(GetPrimarySpeed()) * THROWABLE_HIT_WALL_BOUNCE_SPEED_REMAINING);

	// Update floor coordinate
	if (IsDirectionVertical(GetPrimaryMovement()))
	{
		auto newFloor = GetPrimaryMovement() == Direction::UP ? BOTTOM_WALL : TOP_WALL; // primary movement flipped in HitWall()
		// Throwable will travel less distance after speed loss
		auto newDistLeft = GetDistanceLeftToTravel() * THROWABLE_HIT_WALL_BOUNCE_SPEED_REMAINING;
		// Negate remaining distance if bouncing off bottom wall
		newDistLeft = GetPrimaryMovement() == Direction::UP ? -newDistLeft : newDistLeft;
		SetFloorCoordinate(newFloor + newDistLeft);
	}
}

void Throwable::FloorBounce()
{
	Assert(ERROR_THROWABLE_FLOOR_BOUNCE, !m_FloorHitTimer.RanOut());

	// Use absolute values to set new speeds
	auto primSpeed = abs(GetPrimarySpeed());
	auto secSpeed = abs(GetSecondarySpeed());

	// No visible vertical secondary movement
	// Store sign with value when vertical secondary movement not visible (sign cannot be determined with no direction)
	// Negative because going UP in the world decrements Y
	if (IsDirectionVertical(GetPrimaryMovement())) { secSpeed = -secSpeed; }

	// Slow enough to roll ?
	if (primSpeed <= THROWABLE_HIT_FLOOR_ROLL_SPEED_MAX)
	{
		// Stop secondary movement so it doesnt bounce again
		SetSecondaryMovement(DEFAULT_DIRECTION);

		// Slow enough to stop ?
		if (primSpeed < THROWABLE_HIT_FLOOR_ROLL_SPEED_MIN) { SetPrimaryMovement(DEFAULT_DIRECTION); }
		// Continue rolling
		else { SetPrimarySpeed(primSpeed * THROWABLE_HIT_FLOOR_ROLL_SPEED_REMAINING); }
	}
	// Bounce
	else
	{
		TurnOppositeDirSec();
		SetPrimarySpeed(primSpeed * THROWABLE_HIT_FLOOR_BOUNCE_SPEED_REMAINING);
		SetSecondarySpeed(secSpeed * THROWABLE_HIT_FLOOR_BOUNCE_SPEED_REMAINING);
	}

	// Adjust floor coordinate
	if (IsDirectionVertical(GetPrimaryMovement()))
	{
		// ------- TIME OF FLIGHT ------- //
		// Vertical displacement equation : y = yo + Uoy*t - 0.5*g*t^2
		// Uoy : initial vertical speed
		// yo  : initial height
		// t   : time
		// g   : gravitational acceleration
		// Bouncing off floor so yo = 0, at y = 0 (hits floor again), t = time of flight : 0.5*g*t^2 - Uoy*t - 0 = 0
		// Quadratic equation : a*t^2 + b*t + c = 0
		// a = 0.5*g, b = -Uoy, c = 0
		auto timeOfFlight = GetPositiveQuadraticSolution(HalfOf(GRAVITY_ACCELERATION), -abs(GetSecondarySpeed()), 0);

		// ------- HORIZONTAL DISPLACEMENT ------- //
		// Horizontal displacement equation : x = Uox*t
		// Uox : initial horizontal speed
		// t   : time
		auto horizDisplacement = GetPrimarySpeed() * timeOfFlight;

		// Add displacement to floor
		SetFloorCoordinate(GetFloorCoordinate() + horizDisplacement);
	}
}

void Throwable::SetFloorCoordinate(float value)
{
	m_FloorCoordinate = value;
}

void Throwable::MakeTrajectoryParabolic(float initialHeight, float maxHeight)
{
	// Adjust initial height to measure from bottom
	initialHeight -= GetBottomAdjustment();

	auto range = GetMaxRange();
	Assert(ERROR_PROJECTILE_MAX_RANGE_NEGATIVE, range >= 0);
	Assert(ERROR_THROWABLE_INITIAL_HEIGHT_GREATER_THAN_MAX, initialHeight <= maxHeight);
	Assert(ERROR_THROWABLE_MAX_HEIGHT_GREATER_THAN_LIMIT, maxHeight <= PROJECTILE_TRAJECTORY_HEIGHT_LIMIT);
	auto verticalDisplacement = CalcDistOneAxis(initialHeight, maxHeight);

	// ------- INITIAL VERTICAL SPEED ------- //
	// According to work-energy theorem : Uy^2 = Uoy^2 - 2*g*y
	// Uoy : initial vertical speed
	// Uy  : vertical speed
	// g   : gravitational acceleration
	// y   : vertical displacement
	// At max height, Uy = 0 : Uoy = sqrt(2*g*y)
	auto initialVerticalSpeed = sqrtf(2 * GRAVITY_ACCELERATION * verticalDisplacement);
	
	// ------- TIME OF FLIGHT ------- //
	// Vertical displacement equation : y = yo + Uoy*t - 0.5*g*t^2
	// Uoy : initial vertical speed
	// yo  : initial height
	// t   : time
	// g   : gravitational acceleration
	// At y = 0 (hits floor), t = time of flight : 0.5*g*t^2 - Uoy*t - yo = 0
	// Quadratic equation : a*t^2 + b*t + c = 0
	// a = 0.5*g, b = -Uoy, c = -yo
	auto timeOfFlight = GetPositiveQuadraticSolution(HalfOf(GRAVITY_ACCELERATION), -initialVerticalSpeed, -initialHeight);

	// ------- INITIAL HORIZONTAL SPEED ------- //
	// Horizontal displacement equation : x = Uox*t
	// Uox : initial horizontal speed
	// t   : time
	// At x = range (hits floor), t = time of flight : Uox = range / t
	auto initialHorizontalSpeed = range / timeOfFlight;
	Assert(ERROR_THROWABLE_TOO_FAST_FOR_PARABOLIC, initialHorizontalSpeed <= THROWABLE_MAX_SPEED);
	SetPrimarySpeed(initialHorizontalSpeed);

	if (IsDirectionVertical(GetPrimaryMovement()))
	{
		// No visible vertical secondary movement
		SetSecondaryMovement(DEFAULT_DIRECTION);
		// Store sign with value when vertical secondary movement not visible (sign cannot be determined with no direction)
		// Negative because going UP in the world decrements Y
		SetSecondarySpeed(-initialVerticalSpeed);

		// Update floor coordinate, adjust to measure from bottom
		SetFloorCoordinate(GetPos().y + GetBottomAdjustment() + (GetPrimaryMovement() == Direction::UP ? -range : range));
	}
	else if (IsDirectionHorizontal(GetPrimaryMovement()))
	{
		// Throwable secondary movement starts off as UP
		SetSecondaryMovement(Direction::UP);
		SetSecondarySpeed(initialVerticalSpeed);
	}
}

void Throwable::ApplyGravity(float dtAsSeconds)
{
	auto oldSpeed = GetSecondarySpeed();
	// Add because going down in the world increments Y 
	auto newSpeed = oldSpeed + GRAVITY_ACCELERATION * dtAsSeconds;

	// Need to start dropping ?
	if (oldSpeed < 0 && newSpeed > 0) { TurnOppositeDirSec(); }

	// Store sign with value when vertical secondary movement not visible (sign cannot be determined with no direction)
	SetSecondarySpeed(GetSecondaryMovement() != DEFAULT_DIRECTION ? abs(newSpeed) : newSpeed);
}

float Throwable::GetBottomAdjustment() const
{
	// Use half sprite height, NOT sprite Y origin as height changes when sprite rotates
	return HalfOf(GetHeight()) - THROWABLE_HIT_FLOOR_DEPTH_OFFSET;
}

sf::Vector2f Throwable::GetSpawnOffset() const
{
	// Use default spawn offset (0.0f, 0.0f) so projectile throwable
	// is spawned at same position where the weapon throwable was at
	return Projectile::GetSpawnOffset();
}

void Throwable::ReachedMaxDistance()
{
	// Do nothing, let it hit floor
}

float Throwable::GetSpawnRotation() const
{
	// Use weapon hold rotation to spawn projectile
	return GetHoldRotation(GetPrimaryMovement());
}

void Throwable::CreateProjectile(Direction facing, float floorCoordinate, float startHeight)
{
	Throw(facing, floorCoordinate, startHeight);
}

bool Throwable::GetLoadedAmmoVisisble() const
{
	return THROWABLE_AMMO_VISIBLE;
}