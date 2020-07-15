#include "TextPrinting.h"
#include "ErrorLogging.h"			// for Assert()
#include "StringManipulation.h"		// for ContainsNewLine()

static sf::Font defaultFont;
static sf::Font symmetricFont;
static bool fontsLoaded = false;

static constexpr auto FONTS_DIRECTORY = "Fonts/";
static constexpr auto FONT_FILE_EXTENSION = ".ttf";
static constexpr auto DEFAULT_FONT_FILENAME = "Petitinho";
static constexpr auto SYMMETRIC_FONT_FILENAME = "Ponderosa";
static constexpr auto NO_OUTLINE_SIZE = 0.0f;
inline static const auto NO_OUTLINE_COLOR = sf::Color::Transparent;
static constexpr auto MAX_ALIGNMENT = 1.0f;
static constexpr auto MAX_TEXT_SIZE = 100;
static constexpr auto MAX_OUTLINE_SIZE = 100.0f;
static constexpr auto MAX_LINE_SPACING = 100.0f;
static constexpr auto ERROR_X_ALIGNMENT = "X ";
static constexpr auto ERROR_Y_ALIGNMENT = "Y ";
static constexpr auto ERROR_ALIGNMENT_OUT_OF_RANGE = "Alignment out of range - value must be between 0.0f and 1.0f.";
static constexpr auto ERROR_SIZE_OUT_OF_RANGE = "Text size out of range - value must be between 1 and 100.";
static constexpr auto ERROR_EMPTY_STRING = "Attempting to print an empty string.";
static constexpr auto ERROR_OUTLINE_THICKNESS_OUT_OF_RANGE = "Outline thickness out of range - value must be between 0.0f and 100.0f.";
static constexpr auto ERROR_LINE_SPACING_OUT_OF_RANGE = "Line spacing out of range - value must be between 0.0f and 100.0f.";

static void LoadFonts()
{
	defaultFont.loadFromFile(GetFontPath(DEFAULT_FONT_FILENAME));
	symmetricFont.loadFromFile(GetFontPath(SYMMETRIC_FONT_FILENAME));
	fontsLoaded = true;
}

std::string GetFontPath(std::string filename)
{
	return std::string(FONTS_DIRECTORY).append(filename).append(FONT_FILE_EXTENSION);
}

void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, sf::Color outlineColor, float outlineThickness, sf::Font& customFont, TextStyle style, float lineSpacing)
{
	sf::Text _text;
	if (!fontsLoaded) { LoadFonts(); }

	Assert(std::string(ERROR_X_ALIGNMENT) + ERROR_ALIGNMENT_OUT_OF_RANGE, alignment.x >= 0.0f && alignment.x <= MAX_ALIGNMENT);
	Assert(std::string(ERROR_Y_ALIGNMENT) + ERROR_ALIGNMENT_OUT_OF_RANGE, alignment.y >= 0.0f && alignment.y <= MAX_ALIGNMENT);
	Assert(ERROR_EMPTY_STRING, text.length() > 0);
	Assert(ERROR_SIZE_OUT_OF_RANGE, size > 0 && size <= MAX_TEXT_SIZE);
	Assert(ERROR_OUTLINE_THICKNESS_OUT_OF_RANGE, outlineThickness >= 0.0f && outlineThickness <= MAX_OUTLINE_SIZE);
	Assert(ERROR_LINE_SPACING_OUT_OF_RANGE, lineSpacing >= 0.0f && lineSpacing <= MAX_LINE_SPACING);

	// Apply given values
	_text.setFont(customFont);
	_text.setCharacterSize(static_cast<unsigned int>(size));
	_text.setStyle(static_cast<sf::Uint32>(style));
	_text.setOutlineColor(outlineColor);
	_text.setOutlineThickness(outlineThickness);
	_text.setFillColor(color);

	// Check string for any NEWLINE characters
	std::pair<bool, size_t> foundNL;
	std::string curLineText;
	sf::FloatRect textBox;
	float curLineVerticalAlignment;
	auto textRemaining = text;
	auto lineCount = 0;

	do
	{
		// Extract text before newline
		foundNL = ContainsNewLine(textRemaining);
		curLineText = textRemaining.substr(0, foundNL.second);
		_text.setString(curLineText);
		if (foundNL.first) { textRemaining = textRemaining.substr(foundNL.second + 1); } //+1 to skip newline character

		// Center, position and print each line separately
		textBox = _text.getLocalBounds();
		_text.setOrigin(sf::Vector2f(textBox.left + roundf(0.5f * (textBox.width)), textBox.top + roundf(0.5f * (textBox.height))));  // round to fix outline distortments
		curLineVerticalAlignment = (alignment.y + lineCount++ * (static_cast<float>(size + lineSpacing) / static_cast<float>(rWindow.getSize().y)));
		_text.setPosition(sf::Vector2f(roundf(rWindow.getSize().x * alignment.x), roundf(rWindow.getSize().y * curLineVerticalAlignment)));  // round to fix outline distortments
		rWindow.draw(_text);
	}
	while (foundNL.first);
}

void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, sf::Color outlineColor, float outlineThickness, FontStyle font, TextStyle style, float lineSpacing)
{
	PrintText(rWindow, alignment, text, size, color, outlineColor, outlineThickness, (font == DEFAULT_FONT_STYLE) ? defaultFont : symmetricFont, style, lineSpacing);
}

void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, sf::Font& customFont, TextStyle style, float lineSpacing)
{
	PrintText(rWindow, alignment, text, size, color, NO_OUTLINE_COLOR, NO_OUTLINE_SIZE, customFont, style, lineSpacing);
}

void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, FontStyle font, TextStyle style, float lineSpacing)
{
	PrintText(rWindow, alignment, text, size, color, (font == DEFAULT_FONT_STYLE) ? defaultFont : symmetricFont, style, lineSpacing);
}