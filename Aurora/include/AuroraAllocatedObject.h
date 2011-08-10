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
#ifndef __AURORA_ALLOCATED_OBJECT_H__
#define __AURORA_ALLOCATED_OBJECT_H__
#include "AuroraPrereqs.h"
#include "AuroraMemoryConfig.h"
#include <nedmalloc/nedmalloc.h>
#include <new>

namespace Aurora
{
	template<typename = DefaultAllocators, size_t Alignment = 0>
	class AllocatedObject;

	template<typename AllocatorType>
	class AllocatedObject<AllocatorType, 0>
	{
	public:
		inline explicit AllocatedObject() { ; }
		inline explicit AllocatedObject(const AllocatedObject&) { ; }

		void* operator new(size_t Size)
		{
			return AllocatorType::type::allocateMemory(Size);
		}

		void* operator new(size_t Size, const char* File, int Line)
		{
			return AllocatorType::type::allocateMemory(Size, static_cast<size_t>(Line), File);
		}

		void* operator new(size_t Size, const std::nothrow_t&)
		{
			return AllocatorType::type::allocateMemory(Size);
		}

		void* operator new(size_t Size, void* Where)
		{
			// delegate to global operator new
			return ::operator new(Size, Where);
		}

		void* operator new[](size_t Size)
		{
			return AllocatorType::type::allocateMemory(Size);
		}

		void* operator new[](size_t Size, const char* File, int Line)
		{
			return AllocatorType::type::allocateMemory(Size, static_cast<size_t>(Line), File);
		}

		void* operator new[](size_t Size, const std::nothrow_t&)
		{
			return AllocatorType::type::allocateMemory(Size, static_cast<size_t>(__LINE__), __FILE__);
		}

		void operator delete(void* Mem)
		{
			AllocatorType::type::deallocateMemory(Mem);
		}

		void operator delete(void* Mem, void*)
		{
			AllocatorType::type::deallocateMemory(Mem);
		}

		void operator delete(void* Mem, const char* File, int Line)
		{
			AllocatorType::type::deallocateMemory(Mem);
		}

		void operator delete[](void* Mem)
		{
			AllocatorType::type::deallocateMemory(Mem);
		}

		void operator delete[](void* Mem, const char*, int)
		{
			AllocatorType::type::deallocateMemory(Mem);
		}
	};

	template<typename AllocatorType>
	class AllocatedObject<AllocatorType, AURORA_SSE_ALIGNMENT_SIZE> : virtual public AllocatedObject<AllocatorType, 0>
	{
	protected:
		typedef typename AllocatorType::template Aligned<AURORA_SSE_ALIGNMENT_SIZE>::type ThisAlloc;

	public:

		inline explicit AllocatedObject() { ; }
		inline explicit AllocatedObject(const AllocatedObject&) { ; }

		void* operator new(size_t Size)
		{
			return ThisAlloc::allocateMemory(Size);
		}

		void* operator new(size_t Size, const char* File, int Line)
		{
			return ThisAlloc::allocateMemory(Size, static_cast<size_t>(Line), File);
		}

		void* operator new(size_t Size, const std::nothrow_t&)
		{
			return ThisAlloc::allocateMemory(Size);
		}

		void* operator new(size_t Size, void* Where)
		{
			// delegate to global operator new
			return ::operator new(Size, Where);
		}

		void* operator new[](size_t Size)
		{
			return ThisAlloc::allocateMemory(Size);
		}

		void* operator new[](size_t Size, const char* File, int Line)
		{
			return ThisAlloc::allocateMemory(Size, static_cast<size_t>(Line), File);
		}

		void* operator new[](size_t Size, const std::nothrow_t&)
		{
			return ThisAlloc::allocateMemory(Size, static_cast<size_t>(__LINE__), __FILE__);
		}

		void operator delete(void* Mem)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete(void* Mem, void*)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete(void* Mem, const char* File, int Line)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete[](void* Mem)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete[](void* Mem, const char*, int)
		{
			ThisAlloc::deallocateMemory(Mem);
		}
	};

	template<typename AllocatorType, size_t Alignment>
	class AllocatedObject : virtual public AllocatedObject<AllocatorType, Alignment / 2>
	{
	protected:
		typedef typename AllocatorType::template Aligned<Alignment>::type ThisAlloc;

	public:
		inline explicit AllocatedObject() { ; }
		inline explicit AllocatedObject(const AllocatedObject&) { ; }

		void* operator new(size_t Size)
		{
			return ThisAlloc::allocateMemory(Size);
		}

		void* operator new(size_t Size, const char* File, int Line)
		{
			return ThisAlloc::allocateMemory(Size, static_cast<size_t>(Line), File);
		}

		void* operator new(size_t Size, const std::nothrow_t&)
		{
			return ThisAlloc::allocateMemory(Size);
		}

		void* operator new(size_t Size, void* Where)
		{
			// delegate to global operator new
			return ::operator new(Size, Where);
		}

		void* operator new[](size_t Size)
		{
			return ThisAlloc::allocateMemory(Size);
		}

		void* operator new[](size_t Size, const char* File, int Line)
		{
			return ThisAlloc::allocateMemory(Size, static_cast<size_t>(Line), File);
		}

		void* operator new[](size_t Size, const std::nothrow_t&)
		{
			return ThisAlloc::allocateMemory(Size, static_cast<size_t>(__LINE__), __FILE__);
		}

		void operator delete(void* Mem)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete(void* Mem, void*)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete(void* Mem, const char* File, int Line)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete[](void* Mem)
		{
			ThisAlloc::deallocateMemory(Mem);
		}

		void operator delete[](void* Mem, const char*, int)
		{
			ThisAlloc::deallocateMemory(Mem);
		}
	};

	typedef AllocatedObject<DefaultAllocators> Alloc;
#	if AURORA_SSE_ENABLED
	typedef AllocatedObject<DefaultAllocators, AURORA_SSE_ALIGNMENT_SIZE> SSEAlloc;
#	else
	typedef Alloc SSEAlloc;
#	endif

};

#endif
