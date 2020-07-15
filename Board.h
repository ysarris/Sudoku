#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

static constexpr auto BOX_SIZE = 3;
static constexpr auto BOARD_SIZE = BOX_SIZE * BOX_SIZE;
static constexpr auto BOARD_CELLS = BOARD_SIZE * BOARD_SIZE;

class Board
{
public:
	Board();
	void GenerateNewSudoku();
	void HideCells(int amountToHide);
	void ClearCellGuess(int index);
	void EnterCellGuess(int index, int value);
	bool IsSolutionCorrect() const;
	bool IsCellHidden(int index) const;
	bool IsCellEmpty(int index) const;
	int CalcIndex(int row, int column) const;
	int CalcRow(int index) const;
	int CalcColumn(int index) const;
	int GetRandomIncorrectCellIndex() const;
	int GetVisibleCellCount() const;
	int GetIncorrectCellCount() const;
	void Draw(sf::RenderWindow& rWindow, float cellSize, sf::Vector2f firstCellPos) const;

private:
	static inline sf::Font ms_CellsFont;
	std::vector<Cell> m_SudokuCells;
	int m_HiddenCellCount;
	int m_EmptyCellCount;
	int m_CorrectCellCount;

	bool IsBoxValid(int value, int index) const;
	bool IsRowValid(int value, int index) const;
	bool IsColumnValid(int value, int index) const;
};

#endif