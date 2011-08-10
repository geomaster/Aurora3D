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
#include "AuroraNedAllocator.h"
#include "AuroraMemoryTracker.h"
#include <nedmalloc/nedmalloc.h>

using namespace Aurora;

// Ordinary allocator
void* NedAllocator::allocateMemory(size_t Bytes)
{
	return allocateMemory(Bytes, __LINE__, __FILE__);
}

void* NedAllocator::allocateMemory(size_t Bytes, size_t Line, const char *File)
{
	void *ptr = nedalloc::nedpmalloc2(static_cast<nedalloc::nedpool*>(NULL), Bytes, 0U, 0U);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->addAllocation(ptr, Line, File);
#	endif
	return ptr;
}

void NedAllocator::deallocateMemory(void* Mem)
{
	nedalloc::nedpfree2(static_cast<nedalloc::nedpool*>(NULL), Mem, 0U);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->removeAllocation(Mem);
#	endif
}

// Aligned allocator (for SSE etc)
void* NedAllocator::allocateAlignedMemory(size_t Bytes, size_t Alignment)
{
	void* ptr = nedalloc::nedpmalloc2(static_cast<nedalloc::nedpool*>(NULL), Bytes, Alignment, 0U);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->addAllocation(ptr, __LINE__, __FILE__);
#	endif
	return ptr;
}

void* NedAllocator::allocateAlignedMemory(size_t Bytes, size_t Alignment, size_t Line, const char *File)
{
	void *ptr = nedalloc::nedpmalloc2(static_cast<nedalloc::nedpool*>(NULL), Bytes, Alignment, 0U);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->addAllocation(ptr, Line, File);
#	endif
	return ptr;
}

void NedAllocator::deallocateAlignedMemory(void* Mem)
{
	nedalloc::nedpfree2(static_cast<nedalloc::nedpool*>(NULL), Mem);
#	if AURORA_MEMORY_TRACKING
	MemoryTracker::getSingletonPtr()->removeAllocation(Mem);
#	endif
}
