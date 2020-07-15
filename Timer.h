#pragma once
#ifndef TIMER_H
#define TIMER_H

static constexpr auto DEFAULT_TIMER_VALUE = 0.0f;

class Timer
{
public:
	Timer(float value = DEFAULT_TIMER_VALUE);
	bool RanOut() const;
	float GetTimeLeft() const;
	void Decrement(float dtAsSeconds);
	void Reset(float value = DEFAULT_TIMER_VALUE);

private:
	float m_Time;
};

#endif