#include "Random.h"
#include "ErrorLogging.h"	// for Assert()

static constexpr auto MAX_ROTATION_DEGREES = 360.0f;
static constexpr auto ERROR_MULTIPLE_OUT_OF_RANGE = "Angle multiple out of range - value must be between 1.0f and 360.0f.";

bool GetRandomBool()
{
	return static_cast<bool>(rand() % 2);
}

Direction GetRandomDirection()
{
	return static_cast<Direction>(rand() % GetNumberOfDirections());
}

int GetRandomNumberBetween(int min, int max)
{
	return min + rand() % (max - min + 1);
}

float GetRandomAngleOfMultiple(float multiple)
{
	Assert(ERROR_MULTIPLE_OUT_OF_RANGE, multiple > 0 && multiple <= MAX_ROTATION_DEGREES);
	auto possibleAngles = static_cast<int>(MAX_ROTATION_DEGREES / multiple);

	return GetRandomNumberBetween(0, possibleAngles - 1) * multiple;
}