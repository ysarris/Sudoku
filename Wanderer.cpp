#include "Wanderer.h"
#include "Random.h"		// for GetRandomDirection()

static constexpr auto WANDERER_CLASS_NAME = "Wanderer";
static constexpr auto WANDERER_MAX_HEALTH = 100.0f;
static constexpr auto WANDERER_DAMAGE = 25;
static constexpr auto WANDERER_KILL_POINTS = 50;
static constexpr auto WANDERER_PRIMARY_SPEED = 25.0f;
static constexpr auto WANDERER_SECONDARY_SPEED = 20.0f;
static constexpr auto WANDERER_WANDER_TIME = 1.5f;
static constexpr auto WANDERER_FOLLOW_NOISE_TIME = 3.0f;
static constexpr auto WANDERER_FOLLOW_SIGHT_TIME = 1.0f;

Wanderer::Wanderer(sf::Vector2f startPos, Direction primMov) :
	Object(startPos),
	Enemy(startPos, primMov, WANDERER_MAX_HEALTH, WANDERER_PRIMARY_SPEED, WANDERER_SECONDARY_SPEED)
{
	SpawnFromGate();
}

void Wanderer::Wander()
{
	// Move in random direction at normal speed
	SetPrimaryMovement(GetRandomDirection());
	SetPrimarySpeed(WANDERER_PRIMARY_SPEED);
	SetSecondaryMovement(DEFAULT_DIRECTION);
	m_DirectionChangeTimer.Reset(WANDERER_WANDER_TIME);
}

int Wanderer::GetKillPoints() const
{
	return WANDERER_KILL_POINTS;
}

float Wanderer::GetMaxHealth() const
{
	return WANDERER_MAX_HEALTH;
}

void Wanderer::Update(float dtAsSeconds)
{
	// Temporarily store previous see/hear states before updating
	auto seePlayerAlready = CanSeePlayer();
	auto hearPlayerAlready = CanHearPlayer();

	// Default Enemy updates
	Enemy::Update(dtAsSeconds);

	// Wanderer updates
	if (IsOutOfGate())
	{
		// See player now ?
		if (CanSeePlayer())
		{
			// Enemies that could not already see player will change direction
			// Enemies that could already see player will change direction if enough time has passed
			if (!seePlayerAlready || m_DirectionChangeTimer.RanOut())
			{
				MoveTowards(msp_Player->GetPos(), WANDERER_PRIMARY_SPEED);
				m_DirectionChangeTimer.Reset(WANDERER_FOLLOW_SIGHT_TIME);
			}
		}
		else
		{
			// Hear player now ?
			if (CanHearPlayer())
			{
				// Enemies that could not already hear player will change direction
				// Enemies that could already hear player will change direction if enough time has passed
				if (!hearPlayerAlready || m_DirectionChangeTimer.RanOut())
				{
					TurnTowards(msp_Player->GetPos());
					m_DirectionChangeTimer.Reset(WANDERER_FOLLOW_NOISE_TIME);
				}
			}
			// Keep wandering
			else if (m_DirectionChangeTimer.RanOut()) { Wander(); }
		}
	}
}

int Wanderer::GetAttackDamage() const
{
	return WANDERER_DAMAGE;
}

const std::string Wanderer::GetClassName() const
{
	return WANDERER_CLASS_NAME;
}