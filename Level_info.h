#pragma once
#ifndef LEVEL_INFO
#define LEVEL_INFO

// Completing odd numbered levels unlocks new weapons
bool IsNewWeaponUnlocked(int level);
bool IsFinalLevel(int level);
int GetFirstLevel();
int GetAmountOfNumbersToHide(int difficulty, int level);
float GetLevelTime(int difficulty);
void ValidateLevel(int level);

#endif