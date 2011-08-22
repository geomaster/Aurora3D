#include <AuroraTransform.h>
#include <cstdlib>
#include <cstdio>
#include <xmmintrin.h>
#include "WinTimer.h"

using namespace Aurora;

int main(int argc, char** argv)
{
	/*Transform *t;
	
	for (int i = 0; i < 20; ++i)
	{
		t = new Transform();
		printf("mod: %d\n", reinterpret_cast<int>(&(t->Rotation)) % sizeof(__m128));

		t->Rotation += t->Rotation;
		delete t;
	}

	system("pause >nul");
	_CrtDumpMemoryLeaks();*/

	/*Timer t;
	const int amt = 20000;
	Real *d[amt];

	t.startCounting();
	for (int i = 0; i < amt; ++i)
	{
		d[i] = Aurora::memalign<Real>(16, 16);
	}
	double timeAllocAligned = t.stopCounting();
	for (int i = 0; i < amt; ++i)
	{
		Aurora::memfree(d[i]);
	}

	t.startCounting();
	for (int i = 0; i < amt; ++i)
	{
		d[i] = (Real*)malloc(16 * sizeof(Real));
	}
	double timeAlloc = t.stopCounting();
	
	t.startCounting();
	for (int i = 0; i < amt; ++i)
	{
		free(d[i]);;
	}
	double timeDealloc = t.stopCounting();
	printf("[amt %d] needed %fs for memalign, %fs for malloc, %fs for free\n", amt, timeAllocAligned, timeAlloc, timeDealloc);*/

	system("pause >nul");
	return 0;
}