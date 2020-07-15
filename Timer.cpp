#include "Timer.h"
#include "ErrorLogging.h"	// for Assert()

static constexpr auto ERROR_TIMER_RESET_VALUE_NEGATIVE = "Timer cannot reset with a negative value - value must be >= 0.";
static constexpr auto ERROR_TIMER_DECREMENT_VALUE_INVALID = "Timer cannot decrement with a negative or zero value - value must be > 0.";

Timer::Timer(float value) :
	m_Time(value)
{
}

bool Timer::RanOut() const
{
	return m_Time <= DEFAULT_TIMER_VALUE;
}

float Timer::GetTimeLeft() const
{
	return RanOut() ? DEFAULT_TIMER_VALUE : m_Time;
}

void Timer::Decrement(float dtAsSeconds)
{
	Assert(ERROR_TIMER_DECREMENT_VALUE_INVALID, dtAsSeconds > DEFAULT_TIMER_VALUE);
	m_Time -= dtAsSeconds;
}

void Timer::Reset(float value)
{
	Assert(ERROR_TIMER_RESET_VALUE_NEGATIVE, value >= DEFAULT_TIMER_VALUE);
	m_Time = value;
}