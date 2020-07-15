#pragma once
#ifndef DIFFICULTY_INFO
#define DIFFICULTY_INFO

#include <string>

// Returns the difficulty as a string
std::string GetDifficultyString(int difficulty);
int GetDefaultDifficulty();
int GetNumberOfDifficulties();
void ValidateDifficulty(int difficulty);

#endif