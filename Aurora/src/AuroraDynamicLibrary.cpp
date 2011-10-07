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
#include "AuroraDynamicLibrary.h"
using namespace Aurora;

DynamicLibrary::DynamicLibrary() : mHandle(NULL), mOpen(false)
{
}

DynamicLibrary::DynamicLibrary(const String& Filename) : mHandle(NULL), mOpen(false)
{
	open(Filename);
}

void DynamicLibrary::open(String Filename)
{
	if (mOpen)
		close();

	AURORA_ASSERT(!mOpen, "close() failed to close the library");

#	if AURORA_DYNLIB_API == 0
	void *handle = dlopen(Filename.c_str(), RTLD_LAZY);
	if (!handle)
		throw LibraryOpenException();
	else mHandle = handle, mOpen = true;
#	elif AURORA_DYNLIB_API == 1
	// Replace the forward slashes
	for (String::iterator it = Filename.begin(); it != Filename.end(); ++it)
		if (*it == '/') *it = '\\';

	HMODULE handle = LoadLibrary(Filename.c_str());
	if (!handle)
		throw LibraryOpenException();
	else mHandle = handle, mOpen = true;
#	endif
}

void DynamicLibrary::close()
{
	if (mOpen)
	{
#		if AURORA_DYNLIB_API == 0
		if (dlclose(mHandle) != 0)
			throw LibraryCloseException();
		else mHandle = NULL, mOpen = false;
#		elif AURORA_DYNLIB_API == 1
		if (FreeLibrary(mHandle) == 0) // Oh, Windows. Always the other way round.
			throw LibraryCloseException();
		else mHandle = NULL, mOpen = false;
#		endif
	}
}

void* DynamicLibrary::getSymbol(String SymbolName)
{
	if (!mOpen)
		throw LibraryNotOpenException();

#	if AURORA_DYNLIB_API == 0
	void *p = dlsym(mHandle, SymbolName.c_str());
	if (!p)
		throw LibraryReadException();
	else return p;
#	elif AURORA_DYNLIB_API == 1
	void *p = GetProcAddress(mHandle, SymbolName.c_str());
	if (!p)
		throw LibraryReadException();
	else return p;
#	endif
}

DynamicLibrary::~DynamicLibrary()
{
	if (mOpen)
		close();
}
