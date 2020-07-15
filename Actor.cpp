#include "Actor.h"
#include "Random.h"			// for GetRandomBool()
#include "Mathematics.h"	// for HalfOf()

static constexpr auto ACTOR_CLASS_NAME = "Actor";
static constexpr auto ACTOR_DYING_FILENAME = "Dying";
static constexpr auto ACTOR_HIT_SOUND_FILENAME = "Hit";
static constexpr auto ACTOR_DYING_TIMER = 2.0f;
static constexpr auto ACTOR_DEAD_HEALTH = 0;
static constexpr auto ACTOR_DEFAULT_ANIM_FRAME = 0;
static constexpr auto ACTOR_TOTAL_ANIM_FRAMES = 2;
static constexpr auto ACTOR_ANIM_DELAY = 0.2f;
static constexpr auto ACTOR_DEFAULT_FACING = Direction::DOWN;
static constexpr auto ERROR_ACTOR_FACING_NONE = "Actor cant be facing Direction::NONE";

float Actor::GetHealth() const
{
	return m_Health;
}

float Actor::GetFeetCoordinate()
{
	return GetPos().y + HalfOf(GetHeight());
}

void Actor::TakeDamage(int value, bool resetHitTimer)
{
	m_Health -= value;

	if (resetHitTimer) { m_HitTimer.Reset(GetHitDelay()); }

	// Did that hit kill actor?
	if (m_Health <= ACTOR_DEAD_HEALTH)
	{
		m_Health = ACTOR_DEAD_HEALTH; // so health bar doesnt go negative
		StartDying(ACTOR_DYING_FILENAME);
	}
	// Not dead yet
	else { PlaySound(ACTOR_HIT_SOUND_FILENAME, mp_HitSound); }
}

void Actor::RestoreHealth(float value)
{
	m_Health += value;

	if (m_Health > GetMaxHealth()) { m_Health = GetMaxHealth(); } // so health bar doesnt go over
}

void Actor::StunFor(float value)
{
	m_StunTimer.Reset(value);
}

void Actor::HitByProjectile(int damage, bool isExploding)
{
	// Only reset actor hit timer when hit by exploding projectile
	TakeDamage(damage, isExploding);
}

void Actor::Update(float dtAsSeconds)
{
	// Default Movable updates
	Movable::Update(dtAsSeconds);

	// Actor updates
	if (GetStatus() == HealthStatus::ALIVE)
	{
		// Not stunned ?
		if (m_StunTimer.RanOut())
		{
			Move(dtAsSeconds);

			// Time for next frame?
			if (m_AnimTimer.RanOut())
			{
				m_AnimFrame = ++m_AnimFrame % ACTOR_TOTAL_ANIM_FRAMES;
				m_AnimTimer.Reset(ACTOR_ANIM_DELAY);
			}
			else { m_AnimTimer.Decrement(dtAsSeconds); }
		}
		else { m_StunTimer.Decrement(dtAsSeconds); }
	}
}

Actor::Actor(sf::Vector2f startPos, float maxHealth, float primSpeed, float secSpeed, Direction primMov, Direction secMov) :
	Object(startPos),
	Movable(startPos, ACTOR_DYING_TIMER, primMov, primSpeed, secMov, secSpeed),
	m_SpawnPos(startPos),
	m_Health(maxHealth),
	m_AnimTimer(ACTOR_ANIM_DELAY),
	m_StunTimer(),
	m_Facing(ACTOR_DEFAULT_FACING),
	m_AnimFrame(ACTOR_DEFAULT_ANIM_FRAME)
{
	AddToDirectory(ACTOR_CLASS_NAME);
}

void Actor::Revive()
{
	// Health
	RestoreHealth(GetMaxHealth());
	SetHealthStatus(DEFAULT_HEALTH_STATUS);

	// Positioning
	SetPos(m_SpawnPos);
	SetFacing(ACTOR_DEFAULT_FACING);
	m_AnimFrame = ACTOR_DEFAULT_ANIM_FRAME;

	// Timers
	m_StunTimer.Reset();
	m_AnimTimer.Reset();
	SetDyingTimer(ACTOR_DYING_TIMER);
}

void Actor::OrientSprite()
{
	// Face direction of primary movement if any, otherwise keep facing same direction as before
	if (GetPrimaryMovement() != Direction::NONE) { SetFacing(GetPrimaryMovement()); }

	auto filename = GetDirectionString((GetFacing() == Direction::LEFT) ? GetOppositeDirection(GetFacing()) : GetFacing());

	SetSprite(filename.append(std::to_string(m_AnimFrame)));

	// Flip right sprite to left
	if (GetFacing() == Direction::LEFT) { FlipSpriteHorizontally(); }
}

void Actor::SetFacing(Direction dir)
{
	Assert(ERROR_ACTOR_FACING_NONE, dir != Direction::NONE);
	m_Facing = dir;
}

Direction Actor::GetFacing() const
{
	return m_Facing;
}

void Actor::Move(float dtAsSeconds)
{
	Movable::Move(dtAsSeconds);
	OrientSprite();
}

float Actor::GetDyingRotation() const
{
	return (GetRandomBool() ? 1 : -1) * ACTOR_DEFAULT_DYING_ROTATION;
}