#ifndef __AURORA_STANDARD_ALLOCATOR_H__
#define __AURORA_STANDARD_ALLOCATOR_H__
#include "AuroraPrereqs.h"

namespace Aurora
{
	class AURORA_LIBRARY StandardAllocator
	{
	public:
		static void *allocateMemory(size_t Bytes);
		static void *allocateMemory(size_t Bytes, size_t Line, const char *File);
		static void deallocateMemory(void* Mem);

		static size_t getMaxSize()
		{
			return -1;
		}
	};

	template<size_t Alignment>
	class AURORA_LIBRARY StandardAlignedAllocator
	{
	public:
		template<size_t NewAlign>
		struct rebind_alignment
		{
			typedef StandardAlignedAllocator<NewAlign> other;
		};

		static void *allocateMemory(size_t Bytes);
		static void *allocateMemory(size_t Bytes, size_t Line, const char *File);
		static void deallocateMemory(void* Mem);

		static size_t getMaxSize()
		{
			return -1;
		}
	};

}

#endif // __AURORA_NED_ALLOCATOR_H__