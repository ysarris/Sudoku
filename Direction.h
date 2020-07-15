#pragma once
#ifndef DIRECTION
#define DIRECTION

#include <string>

enum class Direction { UP, DOWN, LEFT, RIGHT, NONE };
static constexpr auto ERROR_UNKNOWN_DIRECTION = "Unknown direction - add case for all directions";

int GetNumberOfDirections();
bool IsDirectionHorizontal(Direction dir);
bool IsDirectionVertical(Direction dir);
Direction GetOppositeDirection(Direction dir);
std::string GetDirectionString(Direction dir);

#endif