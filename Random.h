#pragma once
#ifndef RANDOM
#define RANDOM

#include "Direction.h"
#include <set>

bool GetRandomBool();
Direction GetRandomDirection();
int GetRandomNumberBetween(int min, int max);
// Returs angle in degrees between 0 and 360
float GetRandomAngleOfMultiple(float multiple);
// Returns a random element from the given set and also removes it from the set
template<typename T>
inline T TakeRandomElementFromSet(std::set<T>& set)
{
	// Advance iterator to random position in set
	auto iterator = set.begin();
	advance(iterator, GetRandomNumberBetween(0, set.size() - 1));

	// Dereference iterator for element
	auto element = *iterator;

	// Remove it from the set
	set.erase(element);

	// Return a copy of the element
	return element;
}

#endif