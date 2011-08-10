#ifndef __AURORA_MEMORY_TRACKER_H__
#define __AURORA_MEMORY_TRACKER_H__
#include "AuroraPrereqs.h"

#if AURORA_MEMORY_TRACKING

#include "AuroraHash.h"
#include <boost/unordered_map.hpp>
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

		typedef unordered_map<void*, MemoryAlloc, Hash<void*> > AllocationMap;
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