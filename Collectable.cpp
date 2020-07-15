#include "Collectable.h"
#include "Random.h"			// for GetRandomAngleOfMultiple()
#include "Mathematics.h"	// for HalfOf() and GetPointOnCircle()
#include "Tile_info.h"		// for TILE_SIZE_PIXELS
#include "Walls_info.h"		// for wall boundaries

static constexpr auto COLLECTABLE_CLASS_NAME = "Collectable";
static constexpr auto COLLECTABLE_SOUND_FILENAME_SPAWN = "Spawn";
static constexpr auto COLLECTABLE_SOUND_FILENAME_DESPAWN = "Despawn";
static constexpr auto COLLECTABLE_SOUND_FILENAME_COLLECT = "Collect";
static constexpr auto COLLECTABLE_SPAWN_ANGLE_MULTIPLE = 45.0f;
static constexpr auto COLLECTABLE_SPAWN_DISTANCE = 5 * TILE_SIZE_PIXELS;

bool Collectable::IsDespawned() const
{
	return m_DespawnTimer.RanOut();
}

bool Collectable::IsCollected() const
{
	return m_Collected;
}

void Collectable::Update(float dtAsSeconds)
{
	if (!(IsCollected() || IsDespawned()))
	{
		m_DespawnTimer.Decrement(dtAsSeconds);
		// Time to despawn ?
		if (IsDespawned()) { PlaySound(COLLECTABLE_SOUND_FILENAME_DESPAWN); }
	}
}

void Collectable::Draw(sf::RenderWindow& rWindow) const
{
	if (!(IsCollected() || IsDespawned())) { Object::Draw(rWindow); }
}

Collectable::Collectable(float despawnTime) :
	m_DespawnTimer(despawnTime),
	m_Collected(false)
{
	AddToDirectory(COLLECTABLE_CLASS_NAME);
}

void Collectable::ValidateSpawnPos(sf::Vector2f playerPos)
{
	sf::Vector2f spawnPos;
	auto positionValid = false;
	auto halfWidth = HalfOf(GetWidth());
	auto halfHeight = HalfOf(GetHeight());

	// Default boundaries
	auto minX = LEFT_WALL + halfWidth, maxX = RIGHT_WALL - halfWidth,
		minY = TOP_WALL + halfHeight, maxY = BOTTOM_WALL - halfHeight;

	while (!positionValid)
	{
		// Try random angle
		auto angle = GetRandomAngleOfMultiple(COLLECTABLE_SPAWN_ANGLE_MULTIPLE);

		// Use circle where player is the center, radius is the spawn distance from player
		spawnPos = GetPointOnCircle(playerPos, COLLECTABLE_SPAWN_DISTANCE, angle);
		
		// Position within bounds ?
		if (spawnPos.x >= minX && spawnPos.x <= maxX && spawnPos.y >= minY && spawnPos.y <= maxY) { positionValid = true; }
	}

	SetPos(spawnPos);
	PlaySound(COLLECTABLE_SOUND_FILENAME_SPAWN);
}

void Collectable::SetToCollected()
{
	m_Collected = true;
	PlaySound(COLLECTABLE_SOUND_FILENAME_COLLECT);
}