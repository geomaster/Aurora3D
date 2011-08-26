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
#ifndef __AURORA_MEMORY_TRACKER_H__
#define __AURORA_MEMORY_TRACKER_H__
#include "AuroraPrereqs.h"

#if AURORA_MEMORY_TRACKING

#include "AuroraHash.h"
#include <STL/HashMap.h>
#include <iostream>

#if AURORA_COMPILER == AURORA_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4251) // gets rid of annoying M$'s warning in VC++ regarding the fact that std::string isn't exported.
#endif

using namespace boost;
namespace Aurora
{
	class AURORA_LIBRARY MemoryTracker
	{
	private:
		static MemoryTracker mSingleton;

	protected:
		struct MemoryAlloc
		{
			String File;
			uint Line;
		};

		// default_type is used because type uses allocators, which in turn creates
		// a stack overflow
		typedef STL::HashMap<void*, MemoryAlloc>::default_type AllocationMap;
		typedef AllocationMap::iterator AllocationMapIterator;

		AllocationMap mAllocations;

	public:
		void addAllocation(void* Mem, uint Line, const char *File);
		void addAllocation(void* Mem);
		void removeAllocation(void* Mem);

		bool hasAllocation(void* Mem);
		uint getNumberAllocations();

		void report(std::ostream& Where);

		static MemoryTracker& getSingleton()
		{
			return mSingleton;
		}

		static MemoryTracker* getSingletonPtr()
		{
			return &mSingleton;
		}
	};
}

#if AURORA_COMPILER == AURORA_COMPILER_MSVC
#pragma warning(pop)
#endif


#endif // AURORA_MEMORY_TRACKING
#endif // __AURORA_MEMORY_TRACKER_H__
