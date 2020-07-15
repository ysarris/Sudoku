#include "StringManipulation.h"
#include "ErrorLogging.h"		// for Assert()

static constexpr auto ERROR_ORIGINAL_WIDTH_GREATER_THAN_FINAL = "Original string's length is greater than the final width - original length needs to be equal or less than final width.";
static constexpr auto ERROR_NO_CHARACTERS_LEFT_TO_FILL_BETWEEN_STRINGS = "No characters left to fill in between strings - sum of both string lengths needs to be less than the final width.";

std::string FillWithCharacters(std::string originalString, char fillCharacter, FillType fillType, int finalWidth)
{
	auto startWidth = static_cast<int>(originalString.size());
	auto charactersToFill = finalWidth - startWidth;
	Assert(ERROR_ORIGINAL_WIDTH_GREATER_THAN_FINAL, charactersToFill >= 0);
	auto newString = originalString;

	if (fillType == FillType::AROUND)
	{
		// Add an extra character for symmetry if odd number
		auto eachSide = ((charactersToFill % 2 == 0) ? charactersToFill : ++charactersToFill) / 2;
		newString.insert(newString.begin(), eachSide, fillCharacter);
		newString.insert(newString.end(), eachSide, fillCharacter);
	}
	else
	{
		auto position = (fillType == FillType::LEADING) ? newString.begin() : newString.end();
		newString.insert(position, charactersToFill, fillCharacter);
	}

	return newString;
}

std::string FillWithCharactersInBetween(std::string string1, std::string string2, char fillCharacter, int finalWidth)
{
	auto width1 = static_cast<int>(string1.size());
	auto width2 = static_cast<int>(string2.size());
	auto charactersToFill = finalWidth - (width1 + width2);
	Assert(ERROR_NO_CHARACTERS_LEFT_TO_FILL_BETWEEN_STRINGS, charactersToFill > 0);

	return FillWithCharacters(string1, fillCharacter, FillType::TRAILING, finalWidth - width2).append(string2);
}

std::pair<bool, size_t> ContainsNewLine(std::string string)
{
	auto posFound = string.find(NEWLINE);
	return std::make_pair(posFound != std::string::npos, posFound);
}