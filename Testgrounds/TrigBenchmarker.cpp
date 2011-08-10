#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "WinTimer.h"

template <typename T>
double benchmarkFunctor(T f)
{
	double twopi = 6.283185307179586476925286766559;
	Timer t;

	double fulltime = 0.0;

	t.startCounting();	
	for (double i = 0.f; i < twopi; i += 0.0000001f)
	{
		volatile double res;
		res = f(i);
	}

	return t.stopCounting();
}

struct sin_c
{
	inline double operator()(double v)
	{
		return sin(v);
	}
};

struct cos_c
{
	inline double operator()(double v)
	{
		return cos(v);
	}
};

struct tan_c
{
	inline double operator()(double v)
	{
		return tan(v);
	}
};

struct atan_c
{
	inline double operator()(double v)
	{
		return atan(v);
	}
};

struct cos_asm
{
	inline double operator()(double v)
	{
		__asm
		{
			fld v;
			fcos;
		}
	}
};

struct sin_asm
{
	inline double operator()(double v)
	{
		__asm
		{
			fld v;
			fsin;
		}
	}
};

struct tan_asm
{
	inline double operator()(double v)
	{
		__asm
		{
			fld v;
			fptan;
			fstp v;
		}
	}
};

struct atan_asm
{
	inline double operator()(double v)
	{
		float a = 1.0;
		__asm
		{
			fld a;
			fld v;
			fpatan;
		}
	}
};

int main()
{
	double fullC = 0.0;
	double CCos = benchmarkFunctor(cos_c());
	printf("C Cos: %f\n", CCos);
	fullC += CCos;
	//printf("Intrinsic Cos: %f\n", benchmarkFunctor(cos_asm()));

	double CSin = benchmarkFunctor(sin_c());
	printf("C Sin: %f\n", CSin);
	fullC += CSin;

	double CTan = benchmarkFunctor(tan_c());
	printf("C Tan: %f\n", CTan);
	fullC += CTan;

	double CATan = benchmarkFunctor(atan_c());
	printf("C atan: %f\n", CATan);
	fullC += CATan;

	double fullASM = 0.0;
	double AsmCos = benchmarkFunctor(cos_asm());
	printf("Asm Cos: %f\n", AsmCos);
	fullASM += AsmCos;

	double AsmSin = benchmarkFunctor(sin_asm());
	printf("Asm Sin: %f\n", AsmSin);
	fullASM += AsmSin;

	double AsmTan = benchmarkFunctor(tan_asm());
	printf("Asm Tan: %f\n", AsmTan);
	fullASM += AsmTan;

	double AsmAtan = benchmarkFunctor(atan_asm());
	printf("Asm Atan: %f\n", AsmAtan);
	fullASM += AsmAtan;

	printf("Full C: %f\nFull ASM: %f", fullC, fullASM);

	system("pause >nul");
	return 0;
}