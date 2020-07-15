#include "Flame.h"
#include "Random.h"		// for GetRandomNumberBetween()

static constexpr auto FLAME_CLASS_NAME = "Flame";
static constexpr auto FLAME_DYING_IMAGE_FILENAME = "Burn";
static constexpr auto FLAME_STATIC_BURN_SOUND_FILENAME = "StaticBurn";
static constexpr auto FLAME_STATIC_GO_OUT_SOUND_FILENAME = "StaticGoOut";
static constexpr auto FLAME_DYING_TIMER_DEFAULT = 0.25f;
static constexpr auto FLAME_DYING_TIMER_HIT_ACTOR = 1.5f;
static constexpr auto FLAME_PRIM_SPEED_MIN = 250.0f;
static constexpr auto FLAME_PRIM_SPEED_MAX = 300.0f;
static constexpr auto FLAME_SEC_SPEED_MAX_ABS = 50.0f;
static constexpr auto FLAME_EXPLOSIVE = true;
static constexpr auto FLAME_MAX_RANGE = 120.0f;
static constexpr auto FLAME_DAMAGE = 30;
static constexpr auto FLAME_SCALE_DELAY = 0.01f;
static constexpr auto FLAME_ALIVE_SCALE_MULTIPLIER = 20;
static constexpr auto FLAME_DYING_SCALE_MIN_MULTIPLIER = 4;
static constexpr auto FLAME_DYING_SCALE_MAX_MULTIPLIER = 7;
static constexpr auto FLAME_DYING_SCALE_MIN = 1.0f;
static constexpr auto FLAME_DYING_SCALE_MAX = 1.5f;
static inline const auto FLAME_SPAWN_OFFSET_UP = sf::Vector2f(0, -8);
static inline const auto FLAME_SPAWN_OFFSET_DOWN = sf::Vector2f(0, 10);
static inline const auto FLAME_SPAWN_OFFSET_LEFT = sf::Vector2f(-16, -3);
static inline const auto FLAME_SPAWN_OFFSET_RIGHT = sf::Vector2f(16, -3);

Flame::Flame(sf::Vector2f startPos, Direction facing) :
	Object(startPos),
	Projectile(startPos, FLAME_DYING_TIMER_DEFAULT, facing),
	m_SizeStatus(DEFAULT_SIZE_STATUS),
	m_ScaleTimer()
{
	ms_Count++;
	SetSprite(FLAME_CLASS_NAME);
	AdjustSpawnPositionAndRotation();
	MakeTrajectorySpread(FLAME_PRIM_SPEED_MIN, FLAME_PRIM_SPEED_MAX, FLAME_SEC_SPEED_MAX_ABS);
	// Start playing burn sound when first flame is created
	if (ms_Count == 1) { PlaySound(FLAME_STATIC_BURN_SOUND_FILENAME, msp_BurnSound, true); }
}

Flame::~Flame()
{
	ms_Count--;
	// Play sound of flame going out when last flame dies out
	if (!(ms_Count > 0)) { PlaySound(FLAME_STATIC_GO_OUT_SOUND_FILENAME, msp_BurnSound); }
}

void Flame::Burn(float time)
{
	// New dying time given ?
	if (time != DEFAULT_TIMER_VALUE) { SetDyingTimer(time); }
	// Restore to default scale
	SetSpriteScale(DEFAULT_SCALE, DEFAULT_SCALE);
	StartDying(FLAME_DYING_IMAGE_FILENAME);
}

void Flame::HitWall(sf::Vector2f hitPos)
{
	Burn();
}

void Flame::HitActor(Actor& actor)
{
	// Not burning already ? Burns longer on actor
	if (GetStatus() == HealthStatus::ALIVE) { Burn(FLAME_DYING_TIMER_HIT_ACTOR); }
	// Default actor hit
	Projectile::HitActor(actor);
}

void Flame::Pause()
{
	PauseSound(msp_BurnSound);
}

void Flame::Resume()
{
	ResumeSound(msp_BurnSound);
}

void Flame::Update(float dtAsSeconds)
{
	// Default projectile updates
	Projectile::Update(dtAsSeconds);

	// Time to grow/shrink ?
	if (m_ScaleTimer.RanOut())
	{
		// Alive flame grows as it travels
		if (GetStatus() == HealthStatus::ALIVE)
			ScaleSpriteBy(dtAsSeconds * FLAME_ALIVE_SCALE_MULTIPLIER, dtAsSeconds * FLAME_ALIVE_SCALE_MULTIPLIER);
		// Burning flame grows/shrinks
		else if (GetStatus() == HealthStatus::DYING)
		{
			// Reached min/max ?
			auto curScale = GetSprite().getScale();
			if (curScale.x < FLAME_DYING_SCALE_MIN || curScale.y < FLAME_DYING_SCALE_MIN) { m_SizeStatus = SizeStatus::GROWING; }
			else if (curScale.x > FLAME_DYING_SCALE_MAX || curScale.y > FLAME_DYING_SCALE_MAX) { m_SizeStatus = SizeStatus::SHRINKING; }
			
			// Determine size change
			auto scale = GetRandomNumberBetween(FLAME_DYING_SCALE_MIN_MULTIPLIER, FLAME_DYING_SCALE_MAX_MULTIPLIER);
			scale = (m_SizeStatus == SizeStatus::GROWING) ? scale : -scale;
			ScaleSpriteBy(dtAsSeconds * scale, dtAsSeconds * scale);
		}
		m_ScaleTimer.Reset(FLAME_SCALE_DELAY);
	}
	else { m_ScaleTimer.Decrement(dtAsSeconds); }
}

sf::Vector2f Flame::GetSpawnOffset() const
{
	switch (GetPrimaryMovement())
	{
	case Direction::UP:
		return FLAME_SPAWN_OFFSET_UP;
		break;
	case Direction::DOWN:
		return FLAME_SPAWN_OFFSET_DOWN;
		break;
	case Direction::LEFT:
		return FLAME_SPAWN_OFFSET_LEFT;
		break;
	case Direction::RIGHT:
		return FLAME_SPAWN_OFFSET_RIGHT;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

float Flame::GetMaxRange() const
{
	return FLAME_MAX_RANGE;
}

bool Flame::GetIsExplosive() const
{
	return FLAME_EXPLOSIVE;
}

int Flame::GetDamage() const
{
	return FLAME_DAMAGE;
}

void Flame::ReachedMaxDistance()
{
	Burn();
}

const std::string Flame::GetClassName() const
{
	return FLAME_CLASS_NAME;
}