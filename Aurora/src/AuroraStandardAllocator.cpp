/*
    This file is part of Aurora Game Engine.

    Aurora Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Aurora Game Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Aurora Game Engine.  If not, see <http://www.gnu.org/licenses/>.

	Copyright (C) David Davidovic (Geomaster) 2011.

*/
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
