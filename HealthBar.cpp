#include "HealthBar.h"

static inline const auto HEALTH_BAR_FILL_COLOR = sf::Color(0, 215, 0);
static inline const auto HEALTH_BAR_EMPTY_COLOR = sf::Color(225, 245, 165, 175);
static inline const auto HEALTH_BAR_OUTLINE_COLOR = sf::Color::Black;
static constexpr auto HEALTH_BAR_OUTLINE_THICKNESS = 3.0f;

HealthBar::HealthBar(sf::Vector2f size, sf::Vector2f pos) :
	m_Size(size)
{
	m_HealthBarFill.setSize(size);
	m_HealthBarFill.setFillColor(HEALTH_BAR_FILL_COLOR);
	m_HealthBarOutline.setSize(size);
	m_HealthBarOutline.setOutlineThickness(HEALTH_BAR_OUTLINE_THICKNESS);
	m_HealthBarOutline.setOutlineColor(HEALTH_BAR_OUTLINE_COLOR);
	m_HealthBarOutline.setFillColor(HEALTH_BAR_EMPTY_COLOR);

	// Center both y origins using the original size height
	// Center the x origin of outline rectangle's and set the left side as the x origin for the fill rectangle
	m_HealthBarFill.setOrigin(0.0f, size.y * 0.5f);
	m_HealthBarOutline.setOrigin(m_HealthBarOutline.getLocalBounds().width * 0.5f, size.y * 0.5f);

	SetPos(pos);
}

void HealthBar::SetPos(sf::Vector2f pos)
{
	// Adjust using outline thickness so position is centered
	pos += sf::Vector2f(HEALTH_BAR_OUTLINE_THICKNESS, HEALTH_BAR_OUTLINE_THICKNESS);
	m_HealthBarOutline.setPosition(pos);
	// Align fill rectangle's x origin with the left side of the outline rectangle
	m_HealthBarFill.setPosition(pos.x - 0.5f * m_HealthBarOutline.getLocalBounds().width, pos.y);
}

void HealthBar::Update(float currentHealth, float fullHealth)
{
	m_HealthBarFill.setSize(sf::Vector2f(m_Size.x * (currentHealth / fullHealth), m_Size.y));
}

void HealthBar::Update(float currentHealth, float fullHealth, sf::Vector2f pos)
{
	Update(currentHealth, fullHealth);
	SetPos(pos);
}

void HealthBar::Draw(sf::RenderWindow& rWindow) const
{
	rWindow.draw(m_HealthBarOutline);
	rWindow.draw(m_HealthBarFill);
}