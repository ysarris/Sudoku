#pragma once
#ifndef SCREENS_INFO
#define SCREENS_INFO

enum class ScreenID { START, NEW_GAME, GAME, PAUSE, LEVEL_END, TUTORIAL, HIGHSCORES, ENTER_HIGHSCORE_NAME, QUIT, CLOSED, BACK };
static constexpr auto ERROR_UNKNOWN_SCREEN_ID = "Unknown ScreenID - add case for all screens.";

#endif