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

#ifndef __AURORA_PREREQS_H__
#define __AURORA_PREREQS_H__
#include "AuroraPlatform.h"
#include <boost/functional/hash.hpp>

namespace Aurora
{
	// The type for real numbers
#	if AURORA_USE_DOUBLE_PRECISION == 1
	typedef double Real;
#	else
	typedef float Real;
#	endif

	// Types for characters
	typedef char NarrowCharacter;
	typedef wchar_t WideCharacter;

	// Types for strings
#	if AURORA_USE_WIDE_CHARACTERS == 0
	typedef NarrowCharacter Character;
#	else
	typedef WideCharacter Character;
#	endif

	typedef unsigned char uchar;
	typedef unsigned int uint;
	typedef unsigned long ulong;

	typedef std::basic_string<Character> String;

	// Careful with this macro! An ordinary matrix may be indexed by m[y][x], but parameters to this
	// macro are x, then y.
	// If you were wondering, LEA is short for Load Effective Address, an x86 instruction that does
	// the same as this macro in the instruction set, but with 1D arrays.
#	define LEA2D(x,y,w)			((x)+(y)*(w))

#	if AURORA_USE_DEFAULT_HASH
#	define AURORA_APPROX_HASH(T)				boost::hash<T>
#	define AURORA_HASH(T)						boost::hash<T>
#	else
#	define AURORA_APPROX_HASH(T)				Aurora::ApproximateHash<T>
#	define AURORA_HASH(T)						Aurora::Hash<T>
#	endif

#	if AURORA_MEMORY_TRACKING
#		define AURORA_NEW						new(__FILE__, __LINE__)
#		define AURORA_PLACEMENT_NEW(place)		new(place)
#		define AURORA_NEW_POD(type)				Aurora::GlobalAllocator<type>::allocate(__FILE__, __LINE__)
#		define AURORA_NEW_POD_ARRAY(type,sz)	Aurora::GlobalAllocator<type>::allocateArray(sz, __FILE__, __LINE__)
#		define AURORA_DELETE					delete
#		define AURORA_DELETE_POD(x, type)		Aurora::GlobalAllocator<type>::deallocate(x)
#		define AURORA_DELETE_POD_ARRAY(x, type)	Aurora::GlobalAllocaotr<type>::deallocateArray(x)
#	else
#		define AURORA_NEW						new
#		define AURORA_PLACEMENT_NEW(place)		new(place)
#		define AURORA_NEW_POD(type)				Aurora::GlobalAllocator<type>::allocate()
#		define AURORA_NEW_POD_ARRAY(type, sz)	Aurora::::GlobalAllocator<type>::allocateArray(sz)
#		define AURORA_DELETE					delete
#		define AURORA_DELETE_POD(x, type)		Aurora::GlobalAllocator<type>::deallocate(x)
#		define AURORA_DELETE_POD_ARRAY(x, type)	Aurora::GlobalAllocator<type>::deallocateArray(x)
#	endif

#	define AURORA_ASSERT(x, Description)		assert((x) && (Description))

#	if AURORA_COMPILER == AURORA_COMPILER_MSVC
	// Disable some warnings.
#	pragma warning(disable: 4251)
#	define _CRT_SECURE_NO_WARNINGS				1
#	endif

	class	Entity;
	class	Exception;
	class	Math;
	class	Matrix3;
	class	Matrix4;
	class	MemoryTracker;
	class	SSE;
	class	Quaternion;
	class	Scene;
	class	SceneManager;
	class	SceneNode;
	struct	Transform;
	class	Vector2D;
	class	Vector3D;
	class	Vector4D;
}

#endif // __AURORA_PREREQS_H__
