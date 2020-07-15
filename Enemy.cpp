#include "Enemy.h"
#include "Random.h"			// for GetRandomBool() and GetRandomNumberBetween()
#include "Mathematics.h"	// for HalfOf(), CalcDistOneAxis(), CalcDistBetweenPoints() and DegreesToRadians()
#include "Tile_info.h"		// for TILE_SIZE, GetPosWithinTile() and IsPosTileCenter()
#include "Walls_info.h"		// for wall boundaries

static constexpr auto ENEMY_CLASS_NAME = "Enemy";
static constexpr auto GROUND_FILENAME = "Ground";
static constexpr auto ENEMY_IDLE_SOUND_FILENAME = "Idle";
static constexpr auto ENEMY_SPAWN_SOUND_FILENAME = "Spawn";
static constexpr auto ENEMY_ATTACK_TIME = 0.15f;
static constexpr auto ENEMY_SPEED_BOOST = 15.0f;
static constexpr auto MIN_SIDE_DISTANCE_FROM_PLAYER = 10;
static constexpr auto ENEMY_DEFAULT_HAS_HEALTH_BAR = false;
static constexpr auto ENEMY_HEALTH_BAR_WIDTH = 60.0f;
static constexpr auto ENEMY_HEALTH_BAR_HEIGHT = 7.0f;
static constexpr auto ENEMY_HEALTH_BAR_HEIGHT_ABOVE_HEAD = 10.0f;
// Hearing / Seeing
static constexpr auto VISION_ANGLE_DEGREES = 15.0f;
static constexpr auto DEFAULT_HEARING_DISTANCE = 380.0f;
static constexpr auto DEFAULT_VISION_DISTANCE_FORWARD = 250.0f;
static constexpr auto DEFAULT_VISION_DISTANCE_PERIPHERAL = 40.0f;
// Spawning
static constexpr auto GROUND_SPAWN_MAX_ENEMY_SIZE = TILE_SIZE_PIXELS;
static constexpr auto GROUND_SPAWN_POS_ON_TILE = 5.0f;
static constexpr auto GROUND_SPAWN_STAGES = 6;
static constexpr auto GROUND_SPAWN_TOTAL_TIME = 2.0f;
static constexpr auto GROUND_SPAWN_TIME_PER_STAGE = GROUND_SPAWN_TOTAL_TIME / (GROUND_SPAWN_STAGES - 1); // -1 because final stage is already fully out
static constexpr auto GATE_SPAWN_MAX_ENEMY_SIZE = TILE_SIZE_PIXELS - GATE_HEIGHT_PIXELS;
static constexpr auto GATE_SPAWN_MAX_EXTRA_DIST = 10;
// Errors
static constexpr auto ERROR_ENEMY_POS_NOT_TILE_CENTER = "Enemy start position not valid - it must be the center of a tile in order for the spawn functions to work correctly.";
static constexpr auto ERROR_ENEMY_POS_NOT_IN_GATE = "Enemy start position not valid for a gate spawn. Use gate position as starting position when creating enemy.";
static constexpr auto ERROR_ENEMY_POS_NOT_IN_GROUND = "Enemy start position not valid for a ground spawn. Use a ground tile position as starting position when creating enemy.";
static constexpr auto ERROR_ENEMY_TOO_BIG_FOR_GATE = "Enemy is too big to spawn from a gate. Use smaller sprite or spawn from ground instead.";
static constexpr auto ERROR_ENEMY_TOO_BIG_FOR_GROUND = "Enemy is too big to spawn from ground. Use smaller sprite, width/height must be not be greater than tile size.";
static constexpr auto ERROR_ENEMY_INVALID_SPAWN = "Enemy was not spawned validly. Use one of the spawn functions SpawnFromGround() or SpawnFromGate() in derived enemy ctor.";
static constexpr auto ERROR_ENEMY_ALREADY_SPAWNED = "Enemy was spawned more than once. Only call a spawn function once.";

void Enemy::SetPlayer(Player* pPlayer)
{
	msp_Player = pPlayer;
}

bool Enemy::IsOutOfGate() const
{
	return m_OutOfGate;
}

bool Enemy::IsOutOfGround() const
{
	return m_OutOfGround;
}

void Enemy::AttackPlayer()
{
	msp_Player->TakeDamage(GetAttackDamage());
	// Reset timer so enemy looks for player again sooner while they're close
	m_DirectionChangeTimer.Reset(ENEMY_ATTACK_TIME);
}

bool Enemy::IsReadyForCollision() const
{
	return m_OutOfGround && (m_OutOfGate || CalcDistBetweenPoints(m_SpawnPos, GetPos()) >= GetMinDistanceToExitGate()) && Movable::IsReadyForCollision();
}

bool Enemy::IsReadyForWallCollision() const
{
	return m_OutOfGround && m_OutOfGate && Movable::IsReadyForWallCollision();
}

void Enemy::HitWall(sf::Vector2f hitPos)
{
	Movable::HitWall(hitPos);
	// Turn away from wall and towards direction of player
	TurnTowards(msp_Player->GetPos());
	// Move health bar ?
	if (HasHealthBar()) { mp_HealthBar->SetPos(sf::Vector2f(GetPos().x, GetPos().y - HalfOf(GetHeight()) - ENEMY_HEALTH_BAR_HEIGHT_ABOVE_HEAD)); }
}

void Enemy::Update(float dtAsSeconds)
{
	Assert(ERROR_ENEMY_INVALID_SPAWN, m_OutOfGate || m_OutOfGround);

	// Default Actor updates
	if (m_OutOfGround) { Actor::Update(dtAsSeconds); } // texture rectangle needs to remain partially hidden while spawning from ground

	// Enemy updates
	if (GetStatus() != HealthStatus::DEAD)
	{
		if (GetStatus() == HealthStatus::ALIVE)
		{
			// Not out of gate yet ?
			if (!m_OutOfGate) { ExitGate(); }
			// Not out of ground yet ?
			else if (!m_OutOfGround)
			{
				if (m_GroundSpawnTimer.RanOut()) { ExitGround(dtAsSeconds); }
				else { m_GroundSpawnTimer.Decrement(dtAsSeconds); }
			}

			if (!m_DirectionChangeTimer.RanOut()) { m_DirectionChangeTimer.Decrement(dtAsSeconds); }

			// Look and listen for player
			LookForPlayer();
			ListenForPlayer();

			// Only idle sound should play on loop
			// Currently playing or need to start ?
			if (SoundIsOnLoop() || !SoundIsPlaying())
			{
				// Adjust volume
				auto distance = CalcDistBetweenPoints(GetPos(), msp_Player->GetPos());
				// Close enough for player to hear ?
				if (distance < msp_Player->GetHearingDistance())
				{
					if (!SoundIsPlaying()) { PlaySound(ENEMY_IDLE_SOUND_FILENAME, true); }
					SetSoundVolume(VOLUME_MAX - (distance / msp_Player->GetHearingDistance() * VOLUME_MAX));
				}
				// Too far
				else { StopSound(); }
			}
			// Play other sounds at full volume
			else { SetSoundVolume(VOLUME_MAX); }
		}

		// Update health bar ?
		if (HasHealthBar() && m_OutOfGate && m_OutOfGround)
		{
			// Not initialized ?
			if (mp_HealthBar == nullptr ) { mp_HealthBar = std::make_unique<HealthBar>(sf::Vector2f(ENEMY_HEALTH_BAR_WIDTH, ENEMY_HEALTH_BAR_HEIGHT), sf::Vector2f(GetPos())); }
			mp_HealthBar->Update(GetHealth(), GetMaxHealth(), sf::Vector2f(GetPos().x, GetPos().y - HalfOf(GetHeight()) - ENEMY_HEALTH_BAR_HEIGHT_ABOVE_HEAD));
		}
	}
}

void Enemy::Draw(sf::RenderWindow& rWindow) const
{
	Object::Draw(rWindow);
	// Draw health bar ?
	if (HasHealthBar() && m_OutOfGround && m_OutOfGate) { mp_HealthBar->Draw(rWindow); }
	// Spawning from ground ?
	if (!m_OutOfGround) { rWindow.draw(ms_GroundLine); }
}

Enemy::Enemy(sf::Vector2f startPos, Direction primMov, float maxHealth, float primSpeed, float secSpeed) :
	Object(startPos),
	Actor(startPos, maxHealth, primSpeed, secSpeed, primMov),
	m_DirectionChangeTimer(),
	m_SeePlayer(false),
	m_HearPlayer(false),
	m_OutOfGate(false),
	m_OutOfGround(false),
	m_FinalHeight(),
	m_GroundSpawnTimer()
{
	Assert(ERROR_ENEMY_POS_NOT_TILE_CENTER, IsPosTileCenter(startPos));
	if (ms_GroundLine.getTexture() == nullptr )
	{
		SetSprite(ms_GroundLine, DEFAULT_POSITION, GROUND_FILENAME);
		ms_GroundSound.setBuffer(SoundHolder::GetSoundBuffer(SoundHolder::GetPath(GROUND_FILENAME, GetDirectory())));
	}
	AddToDirectory(ENEMY_CLASS_NAME); // Add after ground line sprite is set
}

void Enemy::SpawnFromGate()
{
	Assert(ERROR_ENEMY_ALREADY_SPAWNED, !(m_OutOfGate || m_OutOfGround));
	Assert(ERROR_ENEMY_POS_NOT_IN_GATE, GetPos().x < LEFT_WALL || GetPos().x > RIGHT_WALL || GetPos().y < TOP_WALL || GetPos().y > BOTTOM_WALL);
	OrientSprite(); // Associate a texture with the sprite
	Assert(ERROR_ENEMY_TOO_BIG_FOR_GATE, GetWidth() < GATE_SPAWN_MAX_ENEMY_SIZE && GetHeight() < GATE_SPAWN_MAX_ENEMY_SIZE);
	
	// Shift x and y positions randomly
	auto shift = static_cast<int>(HalfOf(TILE_SIZE_PIXELS));
	SetPosX(GetPos().x + static_cast<float>(GetRandomNumberBetween(-shift, shift)));
	SetPosY(GetPos().y + static_cast<float>(GetRandomNumberBetween(-shift, shift)));

	// Default boundaries
	auto minX = HalfOf(GetWidth()), maxX = TILE_SIZE_PIXELS - minX,
		minY = HalfOf(GetHeight()), maxY = TILE_SIZE_PIXELS - minY;

	// Adjust boundaries depending on gate orientation
	// Determine gate orientation using direction enemy is facing
	switch (GetFacing())
	{
	case Direction::UP:
		minY += GATE_HEIGHT_PIXELS;
		break;
	case Direction::DOWN:
		maxY -= GATE_HEIGHT_PIXELS;
		break;
	case Direction::LEFT:
		minX += GATE_HEIGHT_PIXELS;
		break;
	case Direction::RIGHT:
		maxX -= GATE_HEIGHT_PIXELS;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}

	// Outside boundaries ?
	auto withinTilePos = GetPosWithinTile(GetPos());
	if (withinTilePos.x < minX) { SetPosX(GetPos().x + minX); }
	else if (withinTilePos.x > maxX) { SetPosX(GetPos().x + maxX - TILE_SIZE_PIXELS); }
	if (withinTilePos.y < minY) { SetPosY(GetPos().y + minY); }
	else if (withinTilePos.y > maxY) { SetPosY(GetPos().y + maxY - TILE_SIZE_PIXELS); }

	// Finalise spawn position
	m_SpawnPos = GetPos();

	// Spawned from gate so already out of ground
	m_OutOfGround = true;
}

void Enemy::SpawnFromGround()
{
	Assert(ERROR_ENEMY_ALREADY_SPAWNED, !(m_OutOfGate || m_OutOfGround));
	Assert(ERROR_ENEMY_POS_NOT_IN_GROUND, GetPos().x > LEFT_WALL || GetPos().x < RIGHT_WALL || GetPos().y > TOP_WALL || GetPos().y < BOTTOM_WALL);
	OrientSprite(); // Associate a texture with the sprite
	m_FinalHeight = GetHeight();
	Assert(ERROR_ENEMY_TOO_BIG_FOR_GROUND, GetWidth() <= GROUND_SPAWN_MAX_ENEMY_SIZE && m_FinalHeight <= GROUND_SPAWN_MAX_ENEMY_SIZE);

	// Position ground line and move enemy below it
	ms_GroundLine.setPosition(GetPos().x, GetPos().y + HalfOf(TILE_SIZE_PIXELS) - GROUND_SPAWN_POS_ON_TILE);
	SetPosY(ms_GroundLine.getPosition().y + HalfOf(m_FinalHeight));

	// Start with rectangle fully hidden "underground"
	ChangeSpriteTextureRectangle(GetWidth(), 0);
	m_GroundSpawnTimer.Reset(GROUND_SPAWN_TIME_PER_STAGE);

	ms_GroundSound.play();

	// Finalise spawn position
	m_SpawnPos = GetPos();

	// Spawned from ground so already out of gate
	m_OutOfGate = true;
}

void Enemy::TurnTowards(sf::Vector2f pos)
{
	// For enemies not moving only change the direction they're facing
	GetPrimaryMovement() == Direction::NONE ? SetFacing(WhereIs(pos).first) : SetPrimaryMovement(WhereIs(pos).first);
}

void Enemy::MoveTowards(sf::Vector2f pos, float speed)
{
	auto here = WhereIs(pos);
	SetPrimaryMovement(here.first);
	SetPrimarySpeed(speed + ENEMY_SPEED_BOOST);

	// Move diagonally ?
	if (GetRandomBool())
	{
		// Dont if already close enough on secondary axis
		auto sideDistance = (IsDirectionHorizontal(here.first) ? CalcDistOneAxis(pos.y, GetPos().y) : CalcDistOneAxis(pos.x, GetPos().x));
		if (sideDistance > MIN_SIDE_DISTANCE_FROM_PLAYER) { SetSecondaryMovement(here.second); }
	}
	else { SetSecondaryMovement(DEFAULT_DIRECTION); }
}

bool Enemy::CanSeePlayer() const
{
	return m_SeePlayer;
}

bool Enemy::CanHearPlayer() const
{
	return m_HearPlayer;
}

// Returns directions the given position is towards in relation to enemy position
std::pair<Direction, Direction> Enemy::WhereIs(sf::Vector2f pos) const
{
	// Calculate distance on each axis
	auto distX = CalcDistOneAxis(pos.x, GetPos().x);
	auto distY = CalcDistOneAxis(pos.y, GetPos().y);

	// Determine the two directions the target position is towards
	// Use horizontal axis for primary direction by default
	auto primaryDir = (pos.x > GetPos().x) ? Direction::RIGHT : Direction::LEFT;
	auto secondaryDir = (pos.y > GetPos().y) ? Direction::DOWN : Direction::UP;

	// Use vertical axis for primary direction if Y distance difference is greater
	if (distY > distX) { std::swap(primaryDir, secondaryDir); }

	return std::make_pair(primaryDir, secondaryDir);
}

void Enemy::ExitGate()
{
	// Enemy must exit gate before they can start to see/hear player and get hit
	// Add a small random value to make it look like each enemy has a slightly different reaction time
	auto distToCover = GetMinDistanceToExitGate() + static_cast<float>(GetRandomNumberBetween(0, GATE_SPAWN_MAX_EXTRA_DIST));

	// Enough distance covered ?
	auto coveredDistance = CalcDistBetweenPoints(m_SpawnPos, GetPos());
	if (coveredDistance > distToCover)
	{
		if (CanSeePlayer() || CanHearPlayer()) { m_OutOfGate = true; }
		// Continue forward an additional random distance
		else
		{
			auto randomDistance = GetRandomNumberBetween(static_cast<int>(HalfOf(TILE_SIZE_PIXELS)), static_cast<int>(TILE_SIZE_PIXELS));
			if (coveredDistance > distToCover + randomDistance) { m_OutOfGate = true; }
		}
	}
}

void Enemy::ExitGround(float dtAsSeconds)
{
	// Increase rectangle to show part of sprite "coming out of ground"
	auto increase = m_FinalHeight / GROUND_SPAWN_STAGES;
	ChangeSpriteTextureRectangle(GetWidth(), GetSprite().getTextureRect().height + increase);

	// Move up in world so visible part starts at ground line 
	SetPosY(GetPos().y - increase);

	// Fully out of ground ?
	if (GetHeight() == m_FinalHeight)
	{
		m_OutOfGround = true;
		PlaySound(ENEMY_SPAWN_SOUND_FILENAME);
	}
	// Not out yet
	else { m_GroundSpawnTimer.Reset(GROUND_SPAWN_TIME_PER_STAGE); }
}

void Enemy::LookForPlayer()
{
	auto withinSight = false;
	auto distFromPlayer = CalcDistBetweenPoints(GetPos(), msp_Player->GetPos());

	// Seen recently and still nearby ?
	if (CanSeePlayer() && distFromPlayer <= HalfOf(GetForwardVisionDistance())) { withinSight = true; }
	// Havent seen recently and/or gone too far
	else
	{
		// Player within seeing distance ?
		if (distFromPlayer <= GetForwardVisionDistance())
		{
			// Facing at least one of the directions player is towards ?
			auto here = WhereIs(msp_Player->GetPos());
			if (GetFacing() == here.first || GetFacing() == here.second)
			{
				// Calculate distances, by default forward distance on horizontal axis, side distance on vertical axis
				auto forwardDist = CalcDistOneAxis(GetPos().x, msp_Player->GetPos().x);
				auto sideDist = CalcDistOneAxis(GetPos().y, msp_Player->GetPos().y);

				// If needed, swap distances to forward distance on vertical axis,  side distance on horizontal axis
				if (IsDirectionVertical(GetFacing())) { std::swap(forwardDist, sideDist); }

				// Use a vision cone defined by the following line equations:
				// y = -+(tan(VISION_ANGLE_DEGREES)*x + GetPeripheralVisionDistance())
				// depending on enemy orientation y and x are the forward distance and side distance (or vise versa),
				// tan(VISION_ANGLE_DEGREES) = slope , GetPeripheralVisionDistance() = y intersect
				// If players's side distance is within the two lines they are within enemy's sight
				auto maxSideDist = forwardDist * tanf(DegreesToRadians(VISION_ANGLE_DEGREES)) + GetPeripheralVisionDistance();
				if (sideDist <= maxSideDist) { withinSight = true; }
			}
		}
	}
	m_SeePlayer = withinSight;
}

void Enemy::ListenForPlayer()
{
	m_HearPlayer = msp_Player->IsMakingNoise() && CalcDistBetweenPoints(GetPos(), msp_Player->GetPos()) <= GetHearingDistance();
}

// Return minimum distance required to get out of gate
float Enemy::GetMinDistanceToExitGate() const
{
	switch (GetFacing())
	{
	case Direction::UP:
		return CalcDistOneAxis(m_SpawnPos.y + HalfOf(GetHeight()), BOTTOM_WALL);
		break;
	case Direction::DOWN:
		return CalcDistOneAxis(m_SpawnPos.y + HalfOf(GetHeight() - GATE_HEIGHT_PIXELS), TOP_WALL);
		break;
	case Direction::LEFT:
		return CalcDistOneAxis(m_SpawnPos.x + HalfOf(GetWidth()), RIGHT_WALL);
		break;
	case Direction::RIGHT:
		return CalcDistOneAxis(m_SpawnPos.x - HalfOf(GetWidth()), LEFT_WALL);
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

bool Enemy::HasHealthBar() const
{
	return ENEMY_DEFAULT_HAS_HEALTH_BAR;
}

float Enemy::GetHearingDistance() const
{
	return DEFAULT_HEARING_DISTANCE;
}

float Enemy::GetForwardVisionDistance() const
{
	return DEFAULT_VISION_DISTANCE_FORWARD;
}

float Enemy::GetPeripheralVisionDistance() const
{
	return DEFAULT_VISION_DISTANCE_PERIPHERAL;
}

const std::string Enemy::GetClassName() const
{
	return ENEMY_CLASS_NAME;
}