#pragma once
#ifndef SPAWN_INFO
#define SPAWN_INFO

float GetHealthPackSpawnTime(int difficulty);
float GetFirstEnemyWaveSpawnTime();
float GetNextEnemyWaveSpawnTime();
int GetLargeEnemySpawnLimit(int difficulty);
int GetEmptyCellsForLargeEnemySpawn(int difficulty, int level, int alreadySpawned);

#endif