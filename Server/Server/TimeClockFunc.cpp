#include "TimeClockFunc.h"

float time_since(LARGE_INTEGER t, LARGE_INTEGER frequency)
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	return float(now.QuadPart - t.QuadPart) / float(frequency.QuadPart / 1000.0f);
}