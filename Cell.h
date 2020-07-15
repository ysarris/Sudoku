#pragma once
#ifndef CELL_H
#define CELL_H

#include <set>

class Cell
{
public:
	Cell();
	void ClearGuess();
	void Hide();
	void Reset();
	void SetValue(int value);
	void SetGuess(int value);
	int GetValue() const;
	int GetGuess() const;
	int GetRandomAvailableCandidate();
	bool IsGuessCorrect() const;
	bool IsGuessClear() const;
	bool IsHidden() const;
	bool RanOutOfCandidates() const;

private:
	std::set<int> m_Candidates;
	int m_Value;
	int m_Guess;
	bool m_Hidden;
};

#endif