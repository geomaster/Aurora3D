#include "WinTimer.h"

void Timer::startCounting()
{
	QueryPerformanceFrequency(&mFreq);
	QueryPerformanceCounter(&mStart);
	mCounting = true;
}

double Timer::peekTimer()
{
	if (!mCounting) return 0.0;

	LARGE_INTEGER k;
	QueryPerformanceCounter(&k);
	return (double)(k.QuadPart - mStart.QuadPart) / (double)mFreq.QuadPart;
}

double Timer::stopCounting()
{
	if (!mCounting) return 0.0;
	double r = peekTimer();
	mCounting = false;

	return r;
}
