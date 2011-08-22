#include <cstdlib>
#include <AuroraSSE.h>
#include <AuroraVector3D.h>
#include "WinTimer.h"
#include <cmath>
#include<iostream>

void dmpBits(float k, unsigned int size)
{
	void *p = &k;
	unsigned char *x = (unsigned char*)p;
	for (unsigned int i = 0; i < size; ++i)
	{
		unsigned char d = x[i];
		for (int j = 0; j < 8; ++j)
		{
			printf("%c", d&128?'1':'0'); 
			d<<=1;
		}
		printf(" ");
	}
}

void mulQ(const float *q1, const float *q2, float *r)
{
	r[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
	r[1] = q1[0] * q2[1] + q1[1] * q2[0] - q1[2] * q2[3] + q1[3] * q2[2];
	r[2] = q1[0] * q2[2] + q1[1] * q2[3] + q1[2] * q2[0] - q1[3] * q2[1];
	r[3] = q1[0] * q2[3] - q1[1] * q2[2] + q1[2] * q2[1] + q1[3] * q2[0];

}

void exTesting()
{
	float k = ::cos(Aurora::HalfPi);
	double d = ::cos(Aurora::HalfPi);
	float k1 = Aurora::Math::cos(Aurora::HalfPi);
	//printf("%f %f %f", k, d, k1);
	std::cout << std::fixed << k1 << " " << k << " " <<::cos(Aurora::HalfPi) << Aurora::Math::cos(Aurora::HalfPi);

	/*
	AURORA_SSE_ALIGN float k[] = { 1.f, 2.f, 3.f, 4.f };
	float lsq = k[0]*k[0]+k[1]*k[1]+k[2]*k[2]+k[3]*k[3];

	AURORA_SSE_ALIGN float r[4];

	Timer t;
	t.startCounting();
	for (int i = 0; i < 5000000; ++i)
	{
		float il = 1.f / Aurora::Math::sqrt(lsq);
		r[0] = k[0] * il;
		r[1] = k[1] * il;
		r[2] = k[2] * il;
		r[3] = k[3] * il;
	}
	double tReal = t.stopCounting();

	t.startCounting();
	for (int i = 0; i < 5000000; ++i)
	{
		Aurora::SSE::packedNormalizeV4(k, lsq,  r);
	}
	double tSSE = t.stopCounting();

	t.startCounting();
	for (int i = 0; i < 5000000; ++i)
	{
		Aurora::SSE::packedNormalizeV4Approx(k, lsq,  r);
	}
	double tSSEApprox = t.stopCounting();

	printf("real: %f, sse: %f, sseapprox: %f", tReal, tSSE, tSSEApprox);
	*/
}
int main()
{
	/*
	AURORA_SSE_ALIGN float r[4];
	AURORA_SSE_ALIGN float d[4] = { 1.f, -2.f, 3.f, -4.f };
	//Aurora::SSE::packedMultiplyQuaternion(d, d, r);
	//mulQ(d,d,r);
	
	/*float d = -0.f;
	dmpBits(d,4);*/
	/*
	Timer t;
	t.startCounting();
	for (long long i = 0; i < 5000000000; ++i)
	{
		Aurora::SSE::packedMultiplyQuaternion(d,d,r);
	}
	double tSSE = t.stopCounting();

	t.startCounting();
	for (long long i = 0; i < 5000000000; ++i)
	{
		mulQ(d,d,r);
	}
	double tReal = t.stopCounting();

	//printf("%f", *r);
	printf("sse: %f real: %f", tSSE, tReal);*/
	system("pause >nul");

	return 0;
}