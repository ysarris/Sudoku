#include "Obstructer.h"

static constexpr auto OBSTRUCTER_CLASS_NAME = "Obstructer";
static constexpr auto OBSTRUCTER_GROW_SOUND_FILENAME = "Grow";
static constexpr auto OBSTRUCTER_SHRINK_SOUND_FILENAME = "Shrink";
static constexpr auto OBSTRUCTER_MAX_HEALTH = 900.0f;
static constexpr auto OBSTRUCTER_DAMAGE = 150;
static constexpr auto OBSTRUCTER_KILL_POINTS = 500;
static constexpr auto OBSTRUCTER_HEARING_DISTANCE = 600.0f;
static constexpr auto OBSTRUCTER_VISION_DISTANCE_FORWARD = 400.0f;
static constexpr auto OBSTRUCTER_VISION_DISTANCE_PERIPHERAL = 300.0f;
static constexpr auto OBSTRUCTER_SCALE_DELAY = 4.5f;
static constexpr auto OBSTRUCTER_SCALE_PER_GROW = 0.5f;
static constexpr auto OBSTRUCTER_MAX_SCALE = 3.0f;
static constexpr auto OBSTRUCTER_TURN_DELAY = 1.0f;
static constexpr auto OBSTRUCTER_HAS_HEALTH_BAR = true;

Obstructer::Obstructer(sf::Vector2f startPos) :
	Object(startPos),
	Enemy(startPos, DEFAULT_DIRECTION, OBSTRUCTER_MAX_HEALTH),
	m_GrowthTimer(OBSTRUCTER_SCALE_DELAY)
{
	SpawnFromGround();
}

int Obstructer::GetKillPoints() const
{
	return OBSTRUCTER_KILL_POINTS;
}

float Obstructer::GetMaxHealth() const
{
	return OBSTRUCTER_MAX_HEALTH;
}

void Obstructer::HitByProjectile(int damage, bool isExploding)
{
	// Time to shrink ?
	if (damage >= GetHealth() && abs(GetSprite().getScale().x) > DEFAULT_SCALE)
	{
		PlaySound(OBSTRUCTER_SHRINK_SOUND_FILENAME);
		ScaleSpriteBy(-OBSTRUCTER_SCALE_PER_GROW, -OBSTRUCTER_SCALE_PER_GROW);
		RestoreHealth(GetMaxHealth());
		// Make damage 0 so health remains full after shrink but hit timer gets reset
		damage = 0;
	}
	
	// Default hit
	Actor::HitByProjectile(damage, isExploding);
	// With every hit player position revealed and growth delayed
	TurnTowards(msp_Player->GetPos());
	m_DirectionChangeTimer.Reset(OBSTRUCTER_TURN_DELAY);
	m_GrowthTimer.Reset(OBSTRUCTER_SCALE_DELAY);
}

void Obstructer::Update(float dtAsSeconds)
{
	// Default Enemy updates
	Enemy::Update(dtAsSeconds);

	// Obstructor updates
	if (GetStatus() == HealthStatus::ALIVE)
	{
		// Time to turn towards player ?
		if ((CanSeePlayer() || CanHearPlayer()) && m_DirectionChangeTimer.RanOut())
		{
			TurnTowards(msp_Player->GetPos());
			m_DirectionChangeTimer.Reset(OBSTRUCTER_TURN_DELAY);
		}

		// Out of ground and not grown to max yet ?
		if (IsOutOfGround() && abs(GetSprite().getScale().x) < OBSTRUCTER_MAX_SCALE)
		{
			// Time to grow ?
			if (m_GrowthTimer.RanOut())
			{
				PlaySound(OBSTRUCTER_GROW_SOUND_FILENAME);
				ScaleSpriteBy(OBSTRUCTER_SCALE_PER_GROW, OBSTRUCTER_SCALE_PER_GROW);
				RestoreHealth(GetMaxHealth());
				m_GrowthTimer.Reset(OBSTRUCTER_SCALE_DELAY);
			}
			else { m_GrowthTimer.Decrement(dtAsSeconds); }
		}
	}
}

bool Obstructer::HasHealthBar() const
{
	return OBSTRUCTER_HAS_HEALTH_BAR;
}

int Obstructer::GetAttackDamage() const
{
	return OBSTRUCTER_DAMAGE;
}

float Obstructer::GetHearingDistance() const
{
	return OBSTRUCTER_HEARING_DISTANCE;
}

float Obstructer::GetForwardVisionDistance() const
{
	return OBSTRUCTER_VISION_DISTANCE_FORWARD;
}

float Obstructer::GetPeripheralVisionDistance() const
{
	return OBSTRUCTER_VISION_DISTANCE_PERIPHERAL;
}

const std::string Obstructer::GetClassName() const
{
	return OBSTRUCTER_CLASS_NAME;
}