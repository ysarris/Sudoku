#pragma once
#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "Object.h"
#include "Timer.h"

class Collectable : public Object
{
public:
	bool IsDespawned() const;
	bool IsCollected() const;
	virtual ~Collectable() noexcept = default;
	virtual void Collect() = 0;
	virtual void Update(float dtAsSeconds) override;
	virtual void Draw(sf::RenderWindow& rWindow) const final override;

protected:
	Collectable(float despawnTime);
	void ValidateSpawnPos(sf::Vector2f playerPos);
	void SetToCollected();

private:
	bool m_Collected;
	Timer m_DespawnTimer;
};

#endif