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
