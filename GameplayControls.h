#pragma once
#ifndef GAMEPLAY_CONTROLS
#define GAMEPLAY_CONTROLS

#include <SFML\Window\Keyboard.hpp>
#include "Direction.h"

// Key press
bool IsShootKeyPressed();
bool IsMoveUpKeyPressed();
bool IsMoveDownKeyPressed();
bool IsMoveLeftKeyPressed();
bool IsMoveRightKeyPressed();
// Key identification
bool IsSwapWeaponKey(sf::Keyboard::Key key);
bool IsClearCellKey(sf::Keyboard::Key key);
bool IsDigitKey(sf::Keyboard::Key key);
bool IsDirectionKey(sf::Keyboard::Key key);
// Key values
int GetDigitForKey(sf::Keyboard::Key key);
Direction GetDirectionForKey(sf::Keyboard::Key key);

#endif