#pragma once
#ifndef TIME_MANIPULATION
#define TIME_MANIPULATION

// Seconds in a minute
static constexpr auto MINUTES = 60;

// Returns the minutes part of given time
int GetMinutes(float timeInSeconds);
// Returns the seconds part of given time
int GetSeconds(float timeInSeconds);

#endif