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
	typedef boost::hash Hash;
	typedef boost::hash ApproximateHash;
#	endif

#	if AURORA_MEMORY_TRACKING
#		define AURORA_NEW						new(__FILE__, __LINE__)
#		define AURORA_NEW_POD(type)				Aurora::GlobalAllocator<type>::allocate(__FILE__, __LINE__)
#		define AURORA_NEW_POD_ARRAY(type,sz)	Aurora::GlobalAllocator<type>::allocateArray(sz, __FILE__, __LINE__)
#		define AURORA_DELETE					delete
#		define AURORA_DELETE_POD(x, type)		Aurora::GlobalAllocator<type>::deallocate(x)
#		define AURORA_DELETE_POD_ARRAY(x, type)	Aurora::GlobalAllocaotr<type>::deallocateArray(x)
#	else
#		define AURORA_NEW						new
#		define AURORA_NEW_POD(type)				Aurora::GlobalAllocator<type>::allocate()
#		define AURORA_NEW_POD_ARRAY(type, sz)	Aurora::::GlobalAllocator<type>::allocateArray(sz)
#		define AURORA_DELETE					delete
#		define AURORA_DELETE_POD(x, type)		Aurora::GlobalAllocator<type>::deallocate(x)
#		define AURORA_DELETE_POD_ARRAY(x, type)	Aurora::GlobalAllocator<type>::deallocateArray(x)
#	endif

	class	Entity;
	class	Exception;
	class	Math;
	class	Matrix3;
	class	Matrix4;
	class	MemoryTracker;
	class	Node;
	class	Pipeline;
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