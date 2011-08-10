#ifndef __AURORA_NED_ALLOCATOR_H__
#define __AURORA_NED_ALLOCATOR_H__
#include "AuroraPrereqs.h"
#include <nedmalloc/nedmalloc.h>

namespace Aurora
{
	class AURORA_LIBRARY NedAllocator
	{
	public:
		static void* allocateMemory(size_t Bytes);
		static void* allocateAlignedMemory(size_t Bytes, size_t Alignment);
		static void* allocateMemory(size_t Bytes, size_t Line, const char *File);
		static void* allocateAlignedMemory(size_t Bytes, size_t Alignment, size_t Line, const char* File);

		static void deallocateMemory(void* Mem);
		static void deallocateAlignedMemory(void* Mem);

		static size_t getMaxSize()
		{
			return -1; // I don't like numeric_limits :(
		}
	};

	template<size_t Alignment = 1>
	class NedAlignedAllocator
	{
	public:
		template<size_t NewAlign>
		struct rebind_alignment
		{
			typedef NedAlignedAllocator<NewAlign> other;
		};

		static void *allocateMemory(size_t Bytes)
		{
			return NedAllocator::allocateAlignedMemory(Bytes, Alignment);
		}


		static void *allocateMemory(size_t Bytes, size_t Line, const char *File)
		{
			return NedAllocator::allocateAlignedMemory(Bytes, Alignment, Line, File);
		}

		static void deallocateMemory(void* Mem)
		{
			return NedAllocator::deallocateAlignedMemory(Mem);
		}

		static size_t getMaxSize()
		{
			return NedAllocator::getMaxSize(); // I don't like numeric_limits :(
		}
	};

}

#endif // __AURORA_NED_ALLOCATOR_H__