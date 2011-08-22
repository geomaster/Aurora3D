#pragma once
#include <Windows.h>

class Timer
{
protected:
	LARGE_INTEGER mStart, mFreq;
	bool mCounting;

public:
	void startCounting();
	double peekTimer();
	double stopCounting();
};