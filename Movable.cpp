#include "Movable.h"

static constexpr auto MOVABLE_DEFAULT_HIT_DELAY = 0.3f;
static constexpr auto MOVABLE_SOUND_FILENAME = "Moving";
static constexpr auto ERROR_MOVABLE_PRIM_SPEED_NEGATIVE = "Movable primary speed cannot be set with a negative value - value must be >= 0 and the sign is determined by primary direction.";
static constexpr auto ERROR_MOVABLE_DYING_TIMER_NEGATIVE = "Movable dying timer cannot be set with a negative value - value must be >= 0.";
// **Secondary speed can be set with negative value, as sign cannot be determined when secondary movement
//   is not visible (= Direction::NONE) due to 2D world (e.g. Throwable moving in vertical primary speed)

HealthStatus Movable::GetStatus() const
{
	return m_HealthStatus;
}

Direction Movable::GetPrimaryMovement() const
{
	return m_PrimaryMovement;
}

Direction Movable::GetSecondaryMovement() const
{
	return m_SecondaryMovement;
}

float Movable::GetDyingTimeLeft() const
{
	return m_DyingTimer.GetTimeLeft();
}

void Movable::SetPrimaryMovement(Direction dir)
{
	m_PrimaryMovement = dir;
}

void Movable::SetSecondaryMovement(Direction dir)
{
	m_SecondaryMovement = dir;
}

bool Movable::IsReadyForCollision() const
{
	return m_HealthStatus == HealthStatus::ALIVE && m_HitTimer.RanOut();
}

bool Movable::IsReadyForWallCollision() const
{
	return m_HealthStatus == HealthStatus::ALIVE;
}

void Movable::HitWall(sf::Vector2f hitPos)
{
	SetPos(hitPos);
}

void Movable::Draw(sf::RenderWindow& rWindow) const
{
	if (GetStatus() != HealthStatus::DEAD) { Object::Draw(rWindow); }
}

Movable::Movable(sf::Vector2f startPos, float dyingTimer, Direction primMov, float primSpeed, Direction secMov, float secSpeed) :
	Object(startPos),
	m_PrimaryMovement(primMov),
	m_PrimarySpeed(primSpeed),
	m_SecondaryMovement(secMov),
	m_SecondarySpeed(secSpeed),
	m_HealthStatus(DEFAULT_HEALTH_STATUS),
	m_DyingTimer(dyingTimer),
	m_HitTimer()
{
	Assert(ERROR_MOVABLE_PRIM_SPEED_NEGATIVE, primSpeed >= DEFAULT_SPEED);
	Assert(ERROR_MOVABLE_DYING_TIMER_NEGATIVE, dyingTimer >= DEFAULT_TIMER_VALUE);
}

float Movable::GetPrimarySpeed() const
{
	// Negate value for UP/LEFT, going UP and LEFT in the world decrements Y and X respectively
	return (m_PrimaryMovement == Direction::UP || m_PrimaryMovement == Direction::LEFT) ? -m_PrimarySpeed : m_PrimarySpeed;
}

float Movable::GetSecondarySpeed() const
{
	// Negate value for UP/LEFT, going UP and LEFT in the world decrements Y and X respectively
	return (m_SecondaryMovement == Direction::UP || m_SecondaryMovement == Direction::LEFT) ? -m_SecondarySpeed : m_SecondarySpeed;
}

void Movable::SetPrimarySpeed(float speed)
{
	Assert(ERROR_MOVABLE_PRIM_SPEED_NEGATIVE, speed >= DEFAULT_SPEED);
	m_PrimarySpeed = speed;
}

void Movable::SetSecondarySpeed(float speed)
{
	m_SecondarySpeed = speed;
}

void Movable::SetHealthStatus(HealthStatus status)
{
	m_HealthStatus = status;
}

void Movable::TurnOppositeDirPrim()
{
	SetPrimaryMovement(GetOppositeDirection(GetPrimaryMovement()));
}

void Movable::TurnOppositeDirSec()
{
	SetSecondaryMovement(GetOppositeDirection(GetSecondaryMovement()));
}

void Movable::StartDying(std::string filename)
{
	m_HealthStatus = HealthStatus::DYING;
	SetSprite(filename);
	RotateSpriteBy(GetDyingRotation());
	StopSound();			// Stop any other sounds first, so if it doesnt have a dying sound to play
	PlaySound(filename);	// it doesnt get stuck in dying state due to a sound on loop already playing
}

void Movable::SetDyingTimer(float value)
{
	m_DyingTimer.Reset(value);
}

float Movable::GetHitDelay() const
{
	return MOVABLE_DEFAULT_HIT_DELAY;
}

void Movable::Move(float dtAsSeconds)
{
	// Update position based on primary movement if any
	if (GetPrimaryMovement() != Direction::NONE)
	{
		if (IsDirectionVertical(GetPrimaryMovement()))
			SetPosY(GetPos().y + GetPrimarySpeed() * dtAsSeconds);
		else
			SetPosX(GetPos().x + GetPrimarySpeed() * dtAsSeconds);

		// Update position based on secondary movement if any
		if (GetSecondaryMovement() != Direction::NONE)
		{
			if (IsDirectionVertical(GetSecondaryMovement()))
				SetPosY(GetPos().y + GetSecondarySpeed() * dtAsSeconds);
			else
				SetPosX(GetPos().x + GetSecondarySpeed() * dtAsSeconds);
		}

		// No sound will play if derived class doesnt have a moving sound
		if (SoundExists(MOVABLE_SOUND_FILENAME) && !SoundIsPlaying()) { PlaySound(MOVABLE_SOUND_FILENAME, true); }
	}
	else
	{
		// Only stop sound if derived class has a moving sound
		// to avoid stopping other sounds that may be playing
		if (SoundExists(MOVABLE_SOUND_FILENAME)) { StopSound(); }
	}
}

void Movable::Update(float dtAsSeconds)
{
	if (GetStatus() == HealthStatus::ALIVE)
	{
		if (!m_HitTimer.RanOut()) { m_HitTimer.Decrement(dtAsSeconds); }
	}
	else if (GetStatus() == HealthStatus::DYING)
	{
		// Finished dying ?
		if (m_DyingTimer.RanOut() && !SoundIsPlaying()) { m_HealthStatus = HealthStatus::DEAD; }
		else { m_DyingTimer.Decrement(dtAsSeconds); }
	}
}