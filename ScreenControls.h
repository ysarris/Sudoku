#pragma once
#ifndef SCREEN_CONTROLS
#define SCREEN_CONTROLS

#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Event.hpp>

// Game
bool WasQuitKeyPressed(sf::Event event);
bool WasWindowCloseAttempted(sf::Event event);
bool WasWindowFocusLost(sf::Event event);
bool WasPauseKeyPressed(sf::Event event);
// Menu
bool IsPrevOptionKeyPressed();
bool IsNextOptionKeyPressed();
bool WasSelectKeyPressed(sf::Event event);
// Page Viewer
bool IsPrevPageKeyPressed();
bool IsNextPageKeyPressed();

#endif