#include "TimeManipulation.h"

int GetMinutes(float timeInSeconds)
{
	return static_cast<int>(timeInSeconds) / MINUTES;
}

int GetSeconds(float timeInSeconds)
{
	return static_cast<int>(timeInSeconds) % MINUTES;
}