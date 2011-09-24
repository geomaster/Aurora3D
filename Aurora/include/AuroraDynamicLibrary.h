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
#ifndef __AURORA_DYNAMIC_LIBRARY_H__
#define __AURORA_DYNAMIC_LIBRARY_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraException.h"

#if AURORA_OS == AURORA_OS_WINDOWS
#	define AURORA_DYNLIB_API		1
#elif AURORA_OS == AURORA_OS_LINUX || AURORA_OS == AURORA_OS_OSX
#	define AURORA_DYNLIB_API		0
#else
#	error ("DynamicLibrary: Operating system unsupported. Abort.")
#endif

#if AURORA_DYNLIB_API == 1
//	And thus Microsoft decided not to group functions by header
//	files, therefore dooming every developer to include over
//	200K of code for only a few functions. Makes sense.
#	include <Windows.h>
#elif AURORA_DYNLIB_API == 0
#	include <dlfcn.h>
#endif

namespace Aurora
{
	// Cross-platform RAII-style dynamic library with symbol retrieval
	//
	// Supports POSIX systems (using the dl* family of functions) and
	// MS Windows (using the Windows API LoadLibrary, GetProcAddress
	// and FreeLibrary). Automatically converts the paths the way
	// APIs like it. Paths should be supplied with forward, not
	// backward slashes (UNIX style, not Win).
	class AURORA_LIBRARY DynamicLibrary : virtual public Alloc
	{
	private:
		DynamicLibrary(const DynamicLibrary& other)
		{
			AURORA_ASSERT(false, "Cannot copy library handles!");
		}

		DynamicLibrary operator=(const DynamicLibrary& other)
		{
			AURORA_ASSERT(false, "Cannot copy library handles!");
			return *this;
		}

	protected:
#		if AURORA_DYNLIB_API == 0
		void *mHandle;
#		elif AURORA_DYNLIB_API == 1
		HMODULE mHandle;
#		endif

		bool mOpen;

	public:
		DynamicLibrary();
		DynamicLibrary(const String& Filename);

		void open(String Filename);
		bool isOpen() const
		{
			return mOpen;
		}

		void close();

		void* getSymbol(String SymbolName);

		~DynamicLibrary();
	};

}

#endif // __AURORA_DYNAMIC_LIBRARY_H__