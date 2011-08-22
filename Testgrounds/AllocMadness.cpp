#include <AuroraMemoryTracker.h>
#include <AuroraAllocatedObject.h>
#include <AuroraMath.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace Aurora;

int main()
{
	Alloc *a1 = AURORA_NEW Alloc();
	Alloc *a2 = AURORA_NEW Alloc[10];
	AlignedAlloc *a3 = AURORA_NEW AlignedAlloc();
	int *pod1 = AURORA_NEW_POD(int);
	int *pod2 = AURORA_NEW_POD_ARRAY(int, 20);
	char* pod3 = AURORA_NEW_POD_ARRAY(char, 30);
	strncpy(pod3, "Hi it's me!", 30);

	*pod1 = (int)(Aurora::Pi * Aurora::Real( 10000.0 ));

	for (int i = 0; i < 20; ++i)
	{
		if (i < 2)
			pod2[i] = 1;
		else
			pod2[i] = pod2[i-1] + pod2[i-2];

		printf("fibonacci[%d] = %d\n", i, pod2[i]);
	}

	printf("str = %s\n", pod3);
	printf("pi's mantissa truncated = %d\n", *pod1);
	printf("\n");

	printf("Memory tracker says:\n");
	std::cout.sync_with_stdio();
	MemoryTracker::getSingleton().report(std::cout);

	AURORA_DELETE a1;
	AURORA_DELETE a2;
	AURORA_DELETE a3;
	AURORA_DELETE_POD(pod1);
	AURORA_DELETE_POD_ARRAY(pod2);
	AURORA_DELETE_POD_ARRAY(pod3);


	printf("\nCleaned up, memory tracker says:\n");
	std::cout.sync_with_stdio();
	MemoryTracker::getSingleton().report(std::cout);

	{ char c; scanf("%c",&c); }
	return 0;
}
