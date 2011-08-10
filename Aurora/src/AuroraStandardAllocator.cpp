#include "AuroraStandardAllocator.h"
#include "AuroraMemoryTracker.h"
#include <malloc.h>
#include <cassert>

using namespace Aurora;

// Ordinary allocator
void* StandardAllocator::allocateMemory(size_t Bytes)
{
	return allocateMemory(Bytes, __LINE__, __FILE__);
}

void* StandardAllocator::allocateMemory(size_t Bytes, size_t Line, const char *File)
{
	void *ptr = malloc(Bytes);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->addAllocation(ptr, Line, File);
#	endif
	return ptr;
}

void StandardAllocator::deallocateMemory(void* Mem)
{
	free(Mem);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->removeAllocation(Mem);
#	endif
}

// Aligned allocator (for SSE etc)
template<size_t Alignment>
void* StandardAlignedAllocator<Alignment>::allocateMemory(size_t Bytes)
{
	return allocateMemory(Bytes, __LINE__, __FILE__);
}

template<size_t Alignment>
void* StandardAlignedAllocator<Alignment>::allocateMemory(size_t Bytes, size_t Line, const char *File)
{
#	if AURORA_OS == AURORA_OS_WINDOWS
	void* ptr = _aligned_malloc(Bytes, alignment);
#	elif AURORA_OS == AURORA_OS_LINUX
	void* ptr;
	posix_memalign(&ptr, Alignment, Bytes);
#	elif AURORA_OS == AURORA_OS_OSX
	if (Alignment > AURORA_SEE_ALIGNMENT_SIZE) assert(!"Mac OS X doesn't align to more than 16byte boundary.");
#	else
	void *ptr = malloc(Bytes);
	assert(!"Can't allocate aligned memory.");
#	endif

#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->addAllocation(ptr, Line, File);
#	endif

	return ptr;
}

template<size_t Alignment>
void StandardAlignedAllocator<Alignment>::deallocateMemory(void* Mem)
{
#	if AURORA_OS == AURORA_OS_WINDOWS
	_aligned_free(Mem);
#	else
	free(Mem);
#	endif

#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->removeAllocation(Mem);
#	endif
}
