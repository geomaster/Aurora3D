#include <AuroraMath.h>
#include <cstdlib>
#include "WinTimer.h"

int main()
{
	/*Aurora::Real angle = Aurora::HalfPi / 1.32;
	printf("cos (%f, %f)\n"
		"sin (%f, %f)\n"
		"tan (%f, %f)\n"
		"atan (%f, %f)\n"
		"asin (%f, %f)\n"
		"acos (%f, %f)\n",
		Aurora::Math::cos(angle),
		::cos(angle),
		Aurora::Math::sin(angle),
		::sin(angle),
		Aurora::Math::tan(angle),
		::tan(angle),
		Aurora::Math::atan(angle),
		::atan(angle),
		Aurora::Math::asin(angle),
		::asin(angle),
		Aurora::Math::acos(angle),
		::acos(angle)
		);*/
	/*
	Timer t;
	t.startCounting();
	for (int i = 0; i < 2000000; ++i)
	{
		float k = double(rand()) / double(RAND_MAX);
		k *= Aurora::TwoPi;
		volatile float res = Aurora::Math::rsqrt(k);
	}
	double t1 = t.stopCounting();

	t.startCounting();
	for (int i = 0; i < 2000000; ++i)
	{
		float k = double(rand()) / double(RAND_MAX);
		k *= Aurora::TwoPi;
		volatile float res = Aurora::Math::rsqrt_e(k);
	}
	double t2 = t.stopCounting();
	*/

	volatile float v1[] = { 4.f, -6.f, 5.f, 0.f };
	volatile float v2[] = { 22.f, 9.f, -10.f, 0.f };
	volatile float v3[] = { 0.f, 0.f, 0.f, 0.f };

	Timer t;
	t.startCounting();
	for (int i = 0; i < 1677721600; ++i)
	{
		__asm
		{
			movups xmm0, v1;
			movups xmm1, v2;
			mulps xmm0, xmm1;
			movups v3, xmm0;
		}
	}
	double sseTime = t.stopCounting();

	t.startCounting();
	for (int i = 0; i < 1677721600; ++i)
	{
		v3[0] = v1[0] * v2[0];
		v3[1] = v1[1] * v2[1];
		v3[2] = v1[2] * v2[2];
		v3[3] = v1[3] * v2[3];
	}
	double cxxTime = t.stopCounting();

	printf("r = (%f, %f, %f, %f)\ncxx vs sse (%f %f)", v3[0], v3[1], v3[2], v3[3], cxxTime, sseTime);
	system("pause >NUL");
}