#pragma once
#ifndef MATHEMATICS
#define MATHEMATICS

#include <SFML\System\Vector2.hpp>

float HalfOf(int value);
float HalfOf(float value);
float HalfOf(unsigned int value);
float DegreesToRadians(float degrees);
float RadiansToDegrees(float radians);
float CalcDistOneAxis(float start, float end);
float CalcDistBetweenPoints(sf::Vector2f pointA, sf::Vector2f pointB);
float GetPositiveQuadraticSolution(float a, float b, float c);
bool IsPointInsideCircle(sf::Vector2f center, float radius, sf::Vector2f point);
sf::Vector2f GetPointOnCircle(sf::Vector2f center, float radius, float angleDegrees);

#endif