#pragma once

#include <windows.h>
#include "TimeClockFunc.h"

class VClock
{
protected:
	bool mStarted;

public:
	bool sleep_granularity_was_set;
	LARGE_INTEGER tick_start_time;
	LARGE_INTEGER clock_frequency;
	LARGE_INTEGER mRealStart;

	VClock();

	bool isStarted() const;

	void Start();

	//tick time for each loop
	void setStartTickTime();

	//get time period since the latest tick loop time in ms
	float getTimeSince_miliSec();

	//get time period since server create
	float getTimeSinceCreate_miliSec(LARGE_INTEGER time);

	//change a tick period to a specific time unit
	float toSeconds(LARGE_INTEGER time);
	float toMiliSeconds(LARGE_INTEGER time);

	~VClock();

};
