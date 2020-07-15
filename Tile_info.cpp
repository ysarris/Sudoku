#include "Tile_info.h"

sf::Vector2f GetPosWithinTile(sf::Vector2f pos)
{
	auto decimals = pos - sf::Vector2f(static_cast<int>(pos.x), static_cast<int>(pos.y));
	auto withinTilePosX = static_cast<int>(pos.x) % static_cast<int>(TILE_SIZE_PIXELS);
	auto withinTilePosY = static_cast<int>(pos.y) % static_cast<int>(TILE_SIZE_PIXELS);
	return sf::Vector2f(withinTilePosX + decimals.x, withinTilePosY + decimals.y);
}

bool IsPosTileCenter(sf::Vector2f pos)
{
	auto withinTilePos = GetPosWithinTile(pos);
	auto tileCenter = 0.5f * TILE_SIZE_PIXELS;
	return (withinTilePos.x == tileCenter && withinTilePos.y == tileCenter);
}