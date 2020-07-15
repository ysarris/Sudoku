#pragma once
#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class HealthBar
{
public:
	HealthBar(sf::Vector2f size, sf::Vector2f pos);
	void SetPos(sf::Vector2f pos);
	void Update(float currentHealth, float fullHealth); // without changing position
	void Update(float currentHealth, float fullHealth, sf::Vector2f pos);
	void Draw(sf::RenderWindow& rWindow) const;

private:
	sf::Vector2f m_Size;
	sf::RectangleShape m_HealthBarFill, m_HealthBarOutline;
};

#endif