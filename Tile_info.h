#pragma once
#ifndef TILE_INFO
#define TILE_INFO

#include <SFML\System\Vector2.hpp>

static constexpr auto TILE_SHEET_FILENAME = "tiles";
// Values depend on tile sheet used
static constexpr auto TILE_SIZE_PIXELS = 63.0f;
static constexpr auto DEFAULT_CELL_TILE_VALUE = 0;
static constexpr auto HIGHLIGHTED_CELL_TILE_VALUE = 1;
static constexpr auto CLOSED_GATE_TILE_VALUE = 3;
static constexpr auto OPEN_GATE_TILE_VALUE = 7;
static constexpr auto GATE_HEIGHT_PIXELS = 13;

sf::Vector2f GetPosWithinTile(sf::Vector2f pos);
bool IsPosTileCenter(sf::Vector2f pos);

#endif