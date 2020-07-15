#pragma once
#ifndef ERROR_LOGGING
#define ERROR_LOGGING

#include <string>

void Assert(std::string errorMessage, bool condition = false);
void DeleteLogFile();

#endif