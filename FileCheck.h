#pragma once
#ifndef FILE_CHECK
#define FILE_CHECK

#include <string>

// Returns false if the file doesnt exist (or cannot be accessed)
bool FileExists(std::string path);

#endif