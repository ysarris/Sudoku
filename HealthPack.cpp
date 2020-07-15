#include "HealthPack.h"

static constexpr auto HEALTH_PACK_CLASS_NAME = "Health Pack";
static constexpr auto HEALTH_PACK_DESPAWN_TIME = 10.0f;
static constexpr auto HEALTH_PACK_VALUE = 200.0f;

HealthPack::HealthPack(Player& rPlayer) :
	Collectable(HEALTH_PACK_DESPAWN_TIME),
	mr_Player(rPlayer)
{
	SetSprite(HEALTH_PACK_CLASS_NAME);
	ValidateSpawnPos(rPlayer.GetPos());
}

void HealthPack::Collect()
{
	SetToCollected();
	mr_Player.RestoreHealth(HEALTH_PACK_VALUE);
}

const std::string HealthPack::GetClassName() const
{
	return HEALTH_PACK_CLASS_NAME;
}