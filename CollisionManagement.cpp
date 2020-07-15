#include "Game.h"
#include "Throwable.h"		// for dynamic_cast<Throwable&> to check floor collision
#include "Random.h"			// for GetRandomNumberBetween()
#include "Mathematics.h"	// for HalfOf(), IsPointInsideCircle(), CalcDistBetweenPoints() and CalcDistOneAxis()
#include "Walls_info.h"		// for wall boundaries
#include "Scores_info.h"	// for CalcKillPoints()

static inline const auto MIN_DISTANCE_FOR_COLLISION_CHECK = HalfOf(sqrtf(2 * SPRITE_MAX_SIZE * SPRITE_MAX_SIZE)); // half the diagonal of largest possible sprite
static constexpr auto PROJECTILE_OFFSET_TOP_WALL_MIN = 15;
static constexpr auto PROJECTILE_OFFSET_TOP_WALL_MAX = 20;
static constexpr auto PROJECTILE_OFFSET_TOP_WALL_ALMOST_FINISHED_TRAVELLING = 3;
static constexpr auto PROJECTILE_OFFSET_BOTTOM_WALL = 6;
static constexpr auto PROJECTILE_OFFSET_LEFT_RIGHT_WALL = 5;
static constexpr auto PLAYER_COVERAGE_REQUIRED_FOR_ENEMY_ATTACK = 0.35f;
static constexpr auto ACTOR_COVERAGE_REQUIRED_FOR_EXPLOSION_HIT = 0.15f;
static constexpr auto ACTOR_COVERAGE_REQUIRED_FOR_PROJECTILE_HIT = 0.3f;
static constexpr auto PROJECTILE_COVERAGE_REQUIRED_FOR_ACTOR_HIT = 0.6f;

// Returns a vector of points to check for collision
static std::vector<sf::Vector2f> GetCollisionPoints(sf::Vector2f center, float widthOffset, float heightOffset)
{
	std::vector<sf::Vector2f> points;

	// Add center first
	points.emplace_back(center);
	// Add corner points
	points.emplace_back(center + sf::Vector2f(widthOffset, heightOffset));
	points.emplace_back(center - sf::Vector2f(widthOffset, heightOffset));
	points.emplace_back(center + sf::Vector2f(-widthOffset, heightOffset));
	points.emplace_back(center - sf::Vector2f(-widthOffset, heightOffset));
	// Add cross points
	points.emplace_back(center + sf::Vector2f(widthOffset, 0));
	points.emplace_back(center - sf::Vector2f(widthOffset, 0));
	points.emplace_back(center + sf::Vector2f(0, heightOffset));
	points.emplace_back(center - sf::Vector2f(0, heightOffset));

	return points;
}

static void CollisionWallsVs(Movable& movable)
{
	// Movable latest details
	auto primMov = movable.GetPrimaryMovement();
	auto secMov = movable.GetSecondaryMovement();
	auto heightOffset = HalfOf(movable.GetHeight());
	auto widthOffset = HalfOf(movable.GetWidth());

	// Default wall offsets
	auto topWallOffset = heightOffset, bottomWallOffset = -heightOffset,
		leftWallOffset = widthOffset, rightWallOffset = -widthOffset;

	// Projectile collision ?
	if (auto* pProj = dynamic_cast<Projectile*>(&movable))
	{
		// Adjust all wall offsets
		topWallOffset -= GetRandomNumberBetween(PROJECTILE_OFFSET_TOP_WALL_MIN, PROJECTILE_OFFSET_TOP_WALL_MAX);
		bottomWallOffset += PROJECTILE_OFFSET_BOTTOM_WALL;
		leftWallOffset -= PROJECTILE_OFFSET_LEFT_RIGHT_WALL;
		rightWallOffset += PROJECTILE_OFFSET_LEFT_RIGHT_WALL;
		// Dont let projectiles moving on horizontal axis hit top wall due to any secondary movement
		if (IsDirectionHorizontal(primMov))
			topWallOffset -= PROJECTILE_TRAJECTORY_HEIGHT_LIMIT;
		// Dont let projectiles moving UP come to a stop and "float" above floor near top wall
		else if (primMov == Direction::UP && pProj->GetDistanceLeftToTravel() < PROJECTILE_OFFSET_TOP_WALL_MAX)
			topWallOffset = -PROJECTILE_OFFSET_TOP_WALL_ALMOST_FINISHED_TRAVELLING;
	}
	// Actor collision ?
	else if (dynamic_cast<Actor*>(&movable) != nullptr)
	{
		// Adjust top wall offset only
		topWallOffset = (movable.GetHeight() < HalfOf(TILE_SIZE_PIXELS)) ? -heightOffset : heightOffset;
	}

	// No need to check top/bottom walls for objects moving only horizontally
	if (!(IsDirectionHorizontal(primMov) && secMov == Direction::NONE))
	{
		auto pos = movable.GetPos();
		auto topWall = TOP_WALL + topWallOffset, bottomWall = BOTTOM_WALL + bottomWallOffset;

		// Ignore objects which have bounced off a wall already and heading in opposite direction
		if (primMov != Direction::DOWN && pos.y < topWall) { movable.HitWall(sf::Vector2f(pos.x, topWall)); }
		else if (primMov != Direction::UP && pos.y > bottomWall) { movable.HitWall(sf::Vector2f(pos.x, bottomWall)); }
	}
	// No need to check left/right walls for objects moving only vertically
	if (!(IsDirectionVertical(primMov) && secMov == Direction::NONE))
	{
		auto pos = movable.GetPos();
		auto rightWall = RIGHT_WALL + rightWallOffset, leftWall = LEFT_WALL + leftWallOffset;

		// Ignore objects which have bounced off a wall already and heading in opposite direction
		if (primMov != Direction::RIGHT && pos.x < leftWall) { movable.HitWall(sf::Vector2f(leftWall, pos.y)); }
		else if (primMov != Direction::LEFT && pos.x > rightWall) { movable.HitWall(sf::Vector2f(rightWall, pos.y)); }
	}
}

static void CollisionFloorVsThrowable(Throwable& throwable)
{
	if (throwable.GetDistanceFromFloor() <= 0) { throwable.HitFloor(); }
}

static void CollisionPlayerVsEnemy(Player& player, Enemy& enemy)
{
	// Player latest details
	auto playerPos = player.GetPos();
	auto playerWidthOffset = HalfOf(player.GetWidth()) - (PLAYER_COVERAGE_REQUIRED_FOR_ENEMY_ATTACK * player.GetWidth());
	auto playerHeightOffset = HalfOf(player.GetHeight()) - (PLAYER_COVERAGE_REQUIRED_FOR_ENEMY_ATTACK * player.GetHeight());

	// Enemy latest details
	auto enemyPos = enemy.GetPos();
	auto enemyWidthOffset = HalfOf(enemy.GetWidth());
	auto enemyHeightOffset = HalfOf(enemy.GetHeight());

	// Use the larger dimension
	auto enemyRadius = enemyWidthOffset >= enemyHeightOffset ? enemyWidthOffset : enemyHeightOffset;

	// Any player collision points inside enemy collision circle ?
	for (auto& point : GetCollisionPoints(playerPos, playerWidthOffset, playerHeightOffset))
	{
		// Exit loop when first collision point is found inside circle
		if (IsPointInsideCircle(enemyPos, enemyRadius, point)) { enemy.AttackPlayer(); return; }
	}
}

static void CollisionActorVsProjectile(Actor& actor, Projectile& projectile)
{
	// Actor and Projectile latest details
	auto actorPos = actor.GetPos();
	auto actorWidthOffset = HalfOf(actor.GetWidth());
	auto actorHeightOffset = HalfOf(actor.GetHeight());
	auto projPos = projectile.GetPos();
	auto projWidthOffset = HalfOf(projectile.GetWidth());
	auto projHeightOffset = HalfOf(projectile.GetHeight());

	// Explosion collision ?
	if (projectile.IsExploding())
	{
		// Adjust actor offsets
		actorWidthOffset -= actor.GetWidth() * ACTOR_COVERAGE_REQUIRED_FOR_EXPLOSION_HIT;
		actorHeightOffset -= actor.GetHeight() * ACTOR_COVERAGE_REQUIRED_FOR_EXPLOSION_HIT;

		// Use the larger dimension
		auto explosionRadius = projWidthOffset >= projHeightOffset ? projWidthOffset : projHeightOffset;

		// Any actor collision points inside explosion circle ?
		for (auto& point : GetCollisionPoints(actorPos, actorWidthOffset, actorHeightOffset))
		{
			// Exit loop when first collision point is found inside circle
			if (IsPointInsideCircle(projPos, explosionRadius, point)) { projectile.HitActor(actor); return; }
		}
	}
	// Projectile collision
	else
	{
		// Adjust actor and projectile offsets based on projectile movement
		if (IsDirectionHorizontal(projectile.GetPrimaryMovement()))
		{
			actorWidthOffset -= actor.GetWidth() * ACTOR_COVERAGE_REQUIRED_FOR_PROJECTILE_HIT;
			projHeightOffset -= projectile.GetHeight() * PROJECTILE_COVERAGE_REQUIRED_FOR_ACTOR_HIT;
		}
		else if (IsDirectionVertical(projectile.GetPrimaryMovement()))
		{
			actorHeightOffset -= actor.GetHeight() * ACTOR_COVERAGE_REQUIRED_FOR_PROJECTILE_HIT;
			projWidthOffset -= projectile.GetWidth() * PROJECTILE_COVERAGE_REQUIRED_FOR_ACTOR_HIT;
		}

		// Close enough for projectile to hit actor ?
		auto distX = CalcDistOneAxis(actorPos.x, projPos.x);
		auto distY = CalcDistOneAxis(actorPos.y, projPos.y);
		auto widthOffset = actorWidthOffset + projWidthOffset;
		auto heightOffset = actorHeightOffset + projHeightOffset;
		if (distX <= widthOffset && distY <= heightOffset) { projectile.HitActor(actor); }
	}
}

static void CollisionPlayerVsCollectable(Player& player, Collectable& collectable)
{
	// Player and Collectable latest details
	auto playerPos = player.GetPos();
	auto collectablePos = collectable.GetPos();
	auto widthOffset = HalfOf(player.GetWidth() + collectable.GetWidth());
	auto heightOffset = HalfOf(player.GetHeight() + collectable.GetHeight());

	// Player close enough to collect ?
	auto distX = CalcDistOneAxis(playerPos.x, collectablePos.x);
	auto distY = CalcDistOneAxis(playerPos.y, collectablePos.y);
	if (distX <= widthOffset && distY <= heightOffset) { collectable.Collect(); }
}

void Game::DetectCollisions()
{
	// WALLS
	// vs Player
	if (mp_Player->IsReadyForWallCollision())
		CollisionWallsVs(*mp_Player);
	// vs Enemies
	for (auto& enemy : m_Enemies)
		if (enemy->IsReadyForWallCollision())
			CollisionWallsVs(*enemy);
	// vs Projectiles
	for (auto& projectile : m_Projectiles)
		if (projectile->IsReadyForWallCollision())
			CollisionWallsVs(*projectile);

	// FLOOR
	// vs Throwables 
	for (auto& projectile : m_Projectiles)
		if (auto* pThrowable = dynamic_cast<Throwable*>(projectile.get()))
			if (pThrowable->IsReadyForFloorCollision())
				CollisionFloorVsThrowable(*pThrowable);

	// PROJECTILES
	// vs Enemies
	for (auto& enemy : m_Enemies)
		for (auto& projectile : m_Projectiles)
			if (enemy->IsReadyForCollision() && projectile->IsReadyForCollision())
			{
				// Ignore enemies too far away
				if (CalcDistBetweenPoints(projectile->GetPos(), enemy->GetPos()) <= MIN_DISTANCE_FOR_COLLISION_CHECK)
				{
					CollisionActorVsProjectile(*enemy, *projectile);

					// Enemy killed by projectile ?
					if (enemy->GetStatus() == HealthStatus::DYING) { m_KillsScore += CalcKillPoints(m_Difficulty, enemy->GetKillPoints()); }
				}
			}

	// Dont check player collisions if level over already
	if (!(HasWon() || HasLost()))
	{
		// PLAYER
		// vs Enemies
		for (auto& enemy : m_Enemies)
			if (mp_Player->IsReadyForCollision() && enemy->IsReadyForCollision())
				// Ignore enemies too far away
				if (CalcDistBetweenPoints(mp_Player->GetPos(), enemy->GetPos()) <= MIN_DISTANCE_FOR_COLLISION_CHECK)
					CollisionPlayerVsEnemy(*mp_Player, *enemy);
		// vs Explosions
		for (auto& projectile : m_Projectiles)
			if (mp_Player->IsReadyForCollision() && projectile->IsExploding() && projectile->IsReadyForCollision())
				// Ignore explosions too far away
				if (CalcDistBetweenPoints(mp_Player->GetPos(), projectile->GetPos()) <= MIN_DISTANCE_FOR_COLLISION_CHECK)
					CollisionActorVsProjectile(*mp_Player, *projectile);
		// vs Collectables
		for (auto& collectable : m_Collectables)
			// Ignore collectables already collected or despawned
			if (!collectable->IsCollected() && !collectable->IsDespawned())
				// Ignore collectables too far away
				if (CalcDistBetweenPoints(mp_Player->GetPos(), collectable->GetPos()) <= MIN_DISTANCE_FOR_COLLISION_CHECK)
					CollisionPlayerVsCollectable(*mp_Player, *collectable);
	}
}