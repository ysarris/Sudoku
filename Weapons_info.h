#pragma once
#ifndef WEAPONS_INFO
#define WEAPONS_INFO

// for weapon IDs
#include "Pistol.h"
#include "Hatchet.h"
#include "Shotgun.h"
#include "Grenade.h"
#include "Bow.h"
#include "RocketLauncher.h"
#include "Flamethrower.h"
#include "FireBottle.h"

#define ALL_WEAPON_IDS {PISTOL_ID, HATCHET_ID, SHOTGUN_ID, GRENADE_ID, BOW_ID , ROCKETLAUNCHER_ID, FLAMETHROWER_ID, FIRE_BOTTLE_ID}
static constexpr auto ERROR_ALL_WEAPONS_UNLOCKED = "All weapons already unlocked, player should not be able unlock all weapons during a single playthrough.";
static constexpr auto ERROR_UNKNOWN_WEAPON_ID = "Unknown weapon ID - add case for all weapons.";

#endif