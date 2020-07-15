#include "Direction.h"
#include "ErrorLogging.h"	// for Assert()

int GetNumberOfDirections()
{
	return 4; // Direction::NONE not counted
}

bool IsDirectionHorizontal(Direction dir)
{
	return (dir == Direction::LEFT || dir == Direction::RIGHT) ? true : false;
}

bool IsDirectionVertical(Direction dir)
{
	return (dir == Direction::UP || dir == Direction::DOWN) ? true : false;
}

Direction GetOppositeDirection(Direction dir)
{
	switch (dir)
	{
	case Direction::UP:
		return Direction::DOWN;
		break;
	case Direction::DOWN:
		return Direction::UP;
		break;
	case Direction::LEFT:
		return Direction::RIGHT;
		break;
	case Direction::RIGHT:
		return Direction::LEFT;
		break;
	case Direction::NONE:
		return Direction::NONE;
		break;
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}

std::string GetDirectionString(Direction dir)
{
	switch (dir)
	{
	case Direction::UP:
		return "UP";
		break;
	case Direction::DOWN:
		return "DOWN";
		break;
	case Direction::LEFT:
		return "LEFT";
		break;
	case Direction::RIGHT:
		return "RIGHT";
		break;
	case Direction::NONE:
	default:
		Assert(ERROR_UNKNOWN_DIRECTION);
		break;
	}
}