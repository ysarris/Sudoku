#include "Mathematics.h"
#define _USE_MATH_DEFINES	// to allow access to math constants
#include <cmath>			// for PI, abs() and sqrtf()

static constexpr auto HALF = 0.5f;

float HalfOf(int value)
{
	return static_cast<float>(value) * HALF;
}

float HalfOf(float value)
{
	return value * HALF;
}

float HalfOf(unsigned int value)
{
	return static_cast<float>(value) * HALF;
}

float DegreesToRadians(float degrees)
{
	return degrees * static_cast<float>(M_PI / 180);
}

float RadiansToDegrees(float radians)
{
	return radians * static_cast<float>(180 / M_PI);
}

float CalcDistOneAxis(float start, float end)
{
	return std::abs(start - end);
}

float CalcDistBetweenPoints(sf::Vector2f pointA, sf::Vector2f pointB)
{
	auto distX = CalcDistOneAxis(pointA.x, pointB.x);
	auto distY = CalcDistOneAxis(pointA.y, pointB.y);
	return std::sqrtf(distX * distX + distY * distY);
}

float GetPositiveQuadraticSolution(float a, float b, float c)
{
	return (-b + std::sqrtf(b * b - 4 * a * c)) / (2 * a);
}

bool IsPointInsideCircle(sf::Vector2f center, float radius, sf::Vector2f point)
{
	return CalcDistBetweenPoints(center, point) <= radius;
}

sf::Vector2f GetPointOnCircle(sf::Vector2f center, float radius, float angleDegrees)
{
	// Parametric Equation of a Circle : x = h + r * cos(t) ,  y = k + r * sin(t)
	// (h, k) = center, r = radius, t = angle between line from point to center and positive x-axis
	return sf::Vector2f(center.x + radius * cosf(DegreesToRadians(angleDegrees)), center.y + radius * sinf(DegreesToRadians(angleDegrees)));
}