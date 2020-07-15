#pragma once
#ifndef WALLS_INFO
#define WALLS_INFO

#include "Board.h"		// for BOARD_SIZE
#include "Tile_info.h"	// for TILE_SIZE_PIXELS

static constexpr auto WALL_THICKNESS_TILES = 1; // update m_TileLayout in Game.h if this value changes
static constexpr auto DEFAULT_OFFSET_TOP_WALL = 4; // default offsets used in collision detection for all objects
static constexpr auto DEFAULT_OFFSET_BOTTOM_WALL = -2;
static constexpr auto TOP_WALL = WALL_THICKNESS_TILES * TILE_SIZE_PIXELS + DEFAULT_OFFSET_TOP_WALL;
static constexpr auto BOTTOM_WALL = (WALL_THICKNESS_TILES + BOARD_SIZE) * TILE_SIZE_PIXELS + DEFAULT_OFFSET_BOTTOM_WALL;
static constexpr auto LEFT_WALL = WALL_THICKNESS_TILES * TILE_SIZE_PIXELS;
static constexpr auto RIGHT_WALL = (WALL_THICKNESS_TILES + BOARD_SIZE) * TILE_SIZE_PIXELS;

#endif