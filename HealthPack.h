#pragma once
#ifndef HEALTHPACK_H
#define HEALTHPACK_H

#include "Collectable.h"
#include "Player.h"

class HealthPack : public Collectable
{
public:
	HealthPack(Player& rPlayer);
	virtual void Collect() final override;

private:
	Player& mr_Player;
	virtual const std::string GetClassName() const final override;
};

#endif