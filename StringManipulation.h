#pragma once
#ifndef STRING_MANIPULATION
#define STRING_MANIPULATION

#include <string>
#include "TextCharacters.h"

enum class FillType { TRAILING, LEADING, AROUND };

std::string FillWithCharacters(std::string originalString, char fillCharacter, FillType fillType, int finalWidth);
std::string FillWithCharactersInBetween(std::string string1, std::string string2, char fillCharacter, int finalWidth);
std::pair<bool, size_t> ContainsNewLine(std::string string);

#endif