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
#include "AuroraMemoryTracker.h"

#if AURORA_MEMORY_TRACKING

using namespace Aurora;

MemoryTracker MemoryTracker::mSingleton;

void MemoryTracker::addAllocation(void* Mem, uint Line, const char* File)
{
	MemoryAlloc alloc = { String(File), Line };
	mAllocations[Mem] = alloc;
}

void MemoryTracker::addAllocation(void* Mem)
{
	addAllocation(Mem, __LINE__, __FILE__);
}

void MemoryTracker::removeAllocation(void* Mem)
{
	AllocationMapIterator it = mAllocations.find(Mem);
	assert(it != mAllocations.end() && "You're attempting to deallocate a non-allocated memory address, what the heck are you doing anyway?");
	mAllocations.erase(it);
}

bool MemoryTracker::hasAllocation(void* Mem)
{
	return (mAllocations.find(Mem) != mAllocations.end());
}

Aurora::uint MemoryTracker::getNumberAllocations()
{
	return mAllocations.size();
}

void MemoryTracker::report(std::ostream& Where)
{
	Where << "***************************\n"
			 "** Memory Tracker Report **\n"
			 "***************************\n"
			 "\n";

	if (!mAllocations.size())
	{
		Where << "No non-deallocated memory detected.";
	}
	else
	{
		for (AllocationMapIterator it = mAllocations.begin(); it != mAllocations.end(); ++it)
		{
			Where << "Memory block at " << it->first << " has not been deallocated.\n"
					 "\tAllocated at: " << it->second.File << ":" << it->second.Line << "\n";
		}

		Where << "\nTotal " << getNumberAllocations() << " non-deallocated blocks\n";
	}
}

#endif // AURORA_MEMORY_TRACKING
