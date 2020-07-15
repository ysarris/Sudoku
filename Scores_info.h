#pragma once
#ifndef SCORES_INFO
#define SCORES_INFO

int CalcSolutionScore(int difficulty, int level);
int CalcTimeScore(int difficulty, int level, int minutesRemaining);
int CalcKillPoints(int difficulty, int enemyKillPoints);

#endif