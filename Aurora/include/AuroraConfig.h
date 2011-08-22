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
#ifndef __AURORA_CONFIG_H__
#define __AURORA_CONFIG_H__

/** Sets whether Aurora uses single- or double- precision floating point types.
***
*** Valid values:	0 - use single precision [default]
***				    1 - use double precision
**/
#define AURORA_USE_DOUBLE_PRECISION						0

/** Sets whether Aurora uses wide characters for its string classes or not.
***
*** Valid values:	0 - use normal-width characters [default]
*** 				1 - use wide characters
**/
#define AURORA_USE_WIDE_CHARACTERS						0

/** Sets whether Aurora supports threading (uses multiple threads of execution
*** to perform computations, decode videos etc.). This requires that boost
*** threading library is available.
***
*** Valid values:	0 - do not use threading
***					1 - use threading [default]
**/
#define AURORA_USE_THREADING							1

/** Sets whether Aurora uses basic processor's intrinsic functions, if available.
*** If this is set to 1, Aurora will use inline assembly for supported compilers
*** and architectures to call processor's built-in functions (for example sin and
*** cos) in order to speed up the computation. When using MSVC, this works only on
*** 32 bit platforms.
***
*** Valid values:	0 - do not use basic intrinsic functions
*** 				1 - use basic intrinsic functions [default]
**/
#define AURORA_USE_INTRINSIC_FUNCTIONS					1

/** Sets whether Aurora uses inline assembly for expensive calculations in order to speed
*** them up. If set to 1, internal computations such as matrix, quaternion and vector
*** manipulations will be compiled directly in the assembly language for supported
*** platforms. When using MSVC, this works only on 32 bit platforms.
***
*** Valid values:	0 - do not use inline assembly
***					1 - use inline assembly for expensive calculations [default]
**/
#define AURORA_USE_INLINE_ASSEMBLY						1

/** Sets the level of caching Aurora uses. This level can be 0, 1 or 2.
*** 0 means no caching at all. This may greatly reduce the memory used by Aurora
*** at runtime, but also make some commonly used queries or computations a bit
*** slower.
*** 1 means normal caching; this will save and reuse some commonly used queries
*** and precompute some commonly fetched results. This will slightly increase
*** Aurora's memory footprint at runtime, but will speed up frequently used
*** calls.
*** 2 means aggressive caching; this will greatly increase the memory footprint
*** but will result in almost everything being cached by Aurora. This will improve
*** running speeds because e.g. some lists and maps will be duplicated to allow fast
*** access by more than one criteria.
***
*** Valid values:	0 - do not cache
***					1 - use normal caching [default]
***					2 - use aggressive caching
**/
#define AURORA_CACHING_LEVEL						1

/** Sets whether Aurora uses standard library hash function for hash maps.
*** Aurora has its own hash functions, optimized for speed and good dispersion
*** among the hash space and platform-independent. These hash functions
*** are implemented using assembly if it is enabled (and the platform is
*** supported).
***
*** Valid values:	0 - use Aurora's hash functions [default]
***					1 - use hash functions from the standard library
**/
#define AURORA_USE_DEFAULT_HASH						0

/** Sets whether Aurora uses SSE extensions for optimizing of standard
*** computations, such as vector operations, if available.  Please note that there
**** is no way for the compiler to determine whether SSE is available or not,
*** so turning this on on a non-SSE processor will cause the compilation
*** to fail or the binary to terribly crash due to unknown opcodes. Please also
*** note that, while these optimizations can speed up some of the code up to 4 times,
*** it introduces some constraints - namely, SSE-aligned data cannot be passed
*** by value as function parameters and it types containing SSE vectors
*** cannot be properly allocated by using normal new and delete operators.
*** This is overcome by Aurora's overloaded new and delete operators, but
*** may cause some trouble if you are not using them on purpose.
***
*** Valid values:	0 - do not use SSE
***					1 - use SSE when available [default]
**/
#define AURORA_USE_SSE							1

/** Sets whether SSE is disabled when compiling in debug mode. While on Linux
*** and Unix-like systems "debug" builds behave the same as the regular ones
*** (except for the symbols) on Windows and MSVC++ there is a great deal
*** of sanity checks performed by both MSVCRT as well as the kernel.
*** These make SSE instructions actually several orders of magnitude slower
*** than the normal approach. With this enabled, Aurora will disable
*** SSE optimizations when it detects a debug build. Has effect only when
*** AURORA_USE_SSE is 1.
***
*** Valid values:	0 - use SSE in debug builds [default]
***					1 - disable SSE in debug builds (while allowing it
***					in finals if turned on)
***/
#define AURORA_DISABLE_DEBUG_SSE				0

/** Sets which memory management Aurora uses. It is highly recommended to use
*** nedmalloc because it offers a multiplatform way for a lot of memory
*** alignment business. And it's fast. You shouldn't use the native allocator
*** except if you really need it and know what you're doing.
***
*** Valid values:	0 - use default memory allocator from system
***					1 - use nedmalloc [default]
**/
#define AURORA_MEMORY_ALLOCATOR					1

/** Sets whether Aurora uses its memory tracker to record memory allocations
*** and deallocations. This helps while debugging Aurora and its components
*** by catching memory leaks. However, it includes a little loss in
*** performance because of the lookups and hash computations. Normally, you
*** would disable this in final builds and enable in debugs.
***
*** Valid values:	0 - enable in debugs, disable in finals [default]
***					1 - always disable
***					2 - always enable
**/
#define AURORA_TRACK_MEMORY						0

#endif // __AURORA_CONFIG_H__
