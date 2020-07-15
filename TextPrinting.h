#pragma once
#ifndef TEXT_PRINTING
#define TEXT_PRINTING

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>

enum class FontStyle { DEFAULT, SYMMETRIC };
enum class TextStyle { REGULAR = sf::Text::Style::Regular, BOLD = sf::Text::Style::Bold };
static constexpr auto DEFAULT_FONT_STYLE = FontStyle::DEFAULT;
static constexpr auto DEFAULT_TEXT_STYLE = TextStyle::REGULAR;
static inline const auto DEFAULT_TEXT_COLOR = sf::Color::White;
static inline const auto DEFAULT_TEXT_OUTLINE_COLOR = sf::Color::Black;

std::string GetFontPath(std::string filename);
void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, sf::Color outlineColor, float outlineThickness, sf::Font& customFont, TextStyle style = DEFAULT_TEXT_STYLE, float lineSpacing = 0.0f);
void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, sf::Color outlineColor, float outlineThickness, FontStyle font = DEFAULT_FONT_STYLE, TextStyle style = DEFAULT_TEXT_STYLE, float lineSpacing = 0.0f);
void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, sf::Font& customFont, TextStyle style = DEFAULT_TEXT_STYLE, float lineSpacing = 0.0f);
void PrintText(sf::RenderWindow& rWindow, sf::Vector2f alignment, std::string text, int size, sf::Color color, FontStyle font = DEFAULT_FONT_STYLE, TextStyle style = DEFAULT_TEXT_STYLE, float lineSpacing = 0.0f);

#endif