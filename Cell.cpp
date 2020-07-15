#include "Cell.h"
#include "Random.h"			// for TakeRandomElementFromSet()
#include "ErrorLogging.h"	// for Assert()

#define INIT_SET {1,2,3,4,5,6,7,8,9}
static constexpr auto CLEAR_CELL_VALUE = 0;
static constexpr auto ERROR_HIDE_BEFORE_VALUE_SET = "Cell has no value yet - must have value in order to hide.";

Cell::Cell()
{
	Reset();
}

void Cell::ClearGuess()
{
	m_Guess = CLEAR_CELL_VALUE;
}

void Cell::Hide()
{
	Assert(ERROR_HIDE_BEFORE_VALUE_SET, m_Value != CLEAR_CELL_VALUE);
	ClearGuess();
	m_Hidden = true;
}

void Cell::Reset()
{
	m_Candidates = INIT_SET;
	m_Value = CLEAR_CELL_VALUE;
	m_Hidden = false;
}

void Cell::SetValue(int value)
{
	m_Value = value;
	m_Guess = value;
}

void Cell::SetGuess(int value)
{
	m_Guess = value;
}

int Cell::GetValue() const
{
	return m_Value;
}

int Cell::GetGuess() const
{
	return m_Guess;
}

int Cell::GetRandomAvailableCandidate()
{
	return TakeRandomElementFromSet(m_Candidates);
}

bool Cell::IsGuessCorrect() const
{
	return m_Value == m_Guess;
}

bool Cell::IsGuessClear() const
{
	return m_Guess == CLEAR_CELL_VALUE;
}

bool Cell::IsHidden() const
{
	return m_Hidden;
}

bool Cell::RanOutOfCandidates() const
{
	return static_cast<int>(m_Candidates.size()) == 0;
}