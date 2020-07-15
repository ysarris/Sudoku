#pragma once
#ifndef HIGHSCORE_INFO
#define HIGHSCORE_INFO

static constexpr auto MAX_HIGHSCORES_STORED = 10;
static constexpr auto RANK_MAX_WIDTH = 3;
static constexpr auto NAME_MAX_WIDTH = 10;
static constexpr auto SCORE_MAX_WIDTH = 7;

int GetMaxScorePossible();

#endif