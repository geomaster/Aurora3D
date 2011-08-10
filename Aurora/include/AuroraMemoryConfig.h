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
#ifndef __AURORA_MEMORY_CONFIG_H__
#define __AURORA_MEMORY_CONFIG_H__
#include "AuroraPrereqs.h"
#include "AuroraNedAllocator.h"
#include "AuroraStandardAllocator.h"
#include "AuroraSTLAllocator.h"
#include <nedmalloc/nedmalloc.h>

// Just a few memory typedefs.

namespace Aurora
{
	struct Allocators
	{
		typedef NedAllocator Ned;
		typedef StandardAllocator Standard;
#		if AURORA_MEMORY_ALLOCATOR == 0 // standard malloc.h
		typedef Allocators::Standard Default;
#		elif AURORA_MEMORY_ALLOCATOR == 1 // nedmalloc
		typedef Allocators::Ned Default;
#		endif
	};

	template<size_t Alignment>
	struct AlignedAllocators
	{
		typedef NedAlignedAllocator<Alignment> Ned;
		typedef StandardAlignedAllocator<Alignment> Standard;
#		if AURORA_MEMORY_ALLOCATOR == 0 // standard
		typedef Standard Default;
#		elif AURORA_MEMORY_ALLOCATOR == 1 // ned
		typedef Ned Default;
#		endif
	};

	template<typename T>
	struct STLAllocators
	{
		typedef STLAllocator<T, Allocators::Ned> Ned;
		typedef STLAllocator<T, Allocators::Standard> Standard;
#		if AURORA_MEMORY_ALLOCATOR == 0 // std
		typedef Standard Default;
#		elif AURORA_MEMORY_ALLOCATOR == 1 // ned
		typedef Ned Default;
#		endif
	};

	template<typename T, size_t Alignment>
	struct STLAlignedAllocators
	{
		typedef STLAllocator<T, typename AlignedAllocators<Alignment>::Ned> Ned;
		typedef STLAllocator<T, typename AlignedAllocators<Alignment>::Standard> Standard;
#		if AURORA_MEMORY_ALLOCATOR == 0 // std
		typedef Standard Default;
#		elif AURORA_MEMORY_ALLOCATOR == 1 // ned
		typedef Ned Default;
#		endif
	};

	struct DefaultAllocators
	{
		typedef Allocators::Default type;

		template<size_t Alignment>
		struct Aligned
		{
			typedef typename AlignedAllocators<Alignment>::Default type;
		};
	};

	struct NedAllocators
	{
		typedef Allocators::Ned type;
		template<size_t Alignment>
		struct Aligned
		{
			typedef typename AlignedAllocators<Alignment>::Ned type;
		};
	};

	struct StandardAllocators
	{
		typedef Allocators::Standard type;
		template<size_t Alignment>
		struct Aligned
		{
			typedef typename AlignedAllocators<Alignment>::Standard type;
		};
	};

	template<typename T>
	class GlobalAllocator
	{
	public:
		static T* allocate()
		{
			return static_cast<T*>(Allocators::Default::allocateMemory(sizeof(T)));
		}

		static T* allocate(const char* File, int Line)
		{
			return static_cast<T*>(Allocators::Default::allocateMemory(sizeof(T), static_cast<size_t>(Line), File));
		}

		static T* allocateArray(size_t Size)
		{
			return static_cast<T*>(Allocators::Default::allocateMemory(Size * sizeof(T)));
		}

		static T* allocateArray(size_t Size, const char* File, int Line)
		{
			return static_cast<T*>(Allocators::Default::allocateMemory(sizeof(T) * Size, static_cast<size_t>(Line), File));
		}

		static T* allocateArray(size_t Size, const std::nothrow_t&)
		{
			return static_cast<T*>(Allocators::Default::allocateMemory(sizeof(T) * Size, static_cast<size_t>(__LINE__), __FILE__));
		}

		static void deallocate(void* Mem)
		{
			Allocators::Default::deallocateMemory(Mem);
		}

		static void deallocate(void* Mem, void*)
		{
			Allocators::Default::deallocateMemory(Mem);
		}

		static void deallocate(void* Mem, const char* File, int Line)
		{
			Allocators::Default::deallocateMemory(Mem);
		}

		static void deallocateArray(void* Mem)
		{
			Allocators::Default::deallocateMemory(Mem);
		}

		static void deallocateArray(void* Mem, const char*, int)
		{
			Allocators::Default::deallocateMemory(Mem);
		}

	};
}
#endif // __AURORA_MEMORY_CONFIG_H__
