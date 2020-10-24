#include "VClock.h"






VClock::VClock()
{
	mStarted = false;

	UINT sleep_granularity_ms = 1;
	sleep_granularity_was_set = timeBeginPeriod(sleep_granularity_ms) == TIMERR_NOERROR;

	QueryPerformanceFrequency(&clock_frequency);
}

bool VClock::isStarted() const
{
	return mStarted;
}

void VClock::Start()
{
	QueryPerformanceFrequency(&clock_frequency);
	QueryPerformanceCounter(&mRealStart);
	mStarted = true;
}

void VClock::setStartTickTime()
{
	QueryPerformanceCounter(&tick_start_time);
}

float VClock::getTimeSince_miliSec()
{
	return time_since(tick_start_time, clock_frequency);
}

float VClock::getTimeSinceCreate_miliSec(LARGE_INTEGER time)
{
	return float(time.QuadPart - mRealStart.QuadPart) / float(clock_frequency.QuadPart/1000.0f);
}

float VClock::toSeconds(LARGE_INTEGER time)
{
	return float(time.QuadPart) / float(clock_frequency.QuadPart);
}

float VClock::toMiliSeconds(LARGE_INTEGER time)
{
	return float(time.QuadPart) / float(clock_frequency.QuadPart/1000.0f);
}

VClock::~VClock()
{
}
