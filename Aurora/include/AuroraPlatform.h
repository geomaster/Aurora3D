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
#ifndef __AURORA_PLATFORM_H__
#define __AURORA_PLATFORM_H__
#include "AuroraConfig.h"
#include <string>
#include <malloc.h>
#include <cassert>

#if defined(DEBUG) || defined(_DEBUG) && !defined(AURORA_DEBUG)
#define AURORA_DEBUG					1
#endif

/**** PLATFORM-RELATED DEFINITIONS ****/

// Compilers
#define AURORA_COMPILER_MSVC			0
#define AURORA_COMPILER_GCC				1
#define AURORA_COMPILER_MINGW			2
#define AURORA_COMPILER_BORLAND			3
#define AURORA_COMPILER_INTEL			4
#define AURORA_COMPILER_UNKNOWN			-1

// Operating systems
#define AURORA_OS_WINDOWS				0
#define AURORA_OS_LINUX					1
#define AURORA_OS_OSX					2
#define AURORA_OS_UNKNOWN				-1

// Processor architectures
#define AURORA_ARCHITECTURE_X86			0
#define AURORA_ARCHITECTURE_X64			1
#define AURORA_ARCHITECTURE_UNKNOWN		-1

// Assembly syntaxes
#define AURORA_SYNTAX_UNAVAILABLE		0
#define AURORA_SYNTAX_ATNT				1
#define AURORA_SYNTAX_INTEL				2

#define AURORA_ASSERT(x)				assert((x))

/**** PLATFORM DETECTION ***/

// Detect the compiler
#if defined(_MSC_VER)
#	define AURORA_COMPILER				AURORA_COMPILER_MSVC
#	define AURORA_COMPILER_VERSION		_MSC_VER
#	define AURORA_ASSEMBLY_SYNTAX		AURORA_SYNTAX_INTEL
#elif defined(__GNUC__)
#	define AURORA_COMPILER				AURORA_COMPILER_GCC
#	define AURORA_COMPILER_VERSION		(__GNUC__ * 100) + __GNUC_MINOR__ * 10
#	define AURORA_ASSEMBLY_SYNTAX		AURORA_SYNTAX_ATNT
#elif defined(__MINGW32__)
#	define AURORA_COMPILER				AURORA_COMPILER_MINGW
#	define AURORA_COMPILER_VERSION		(__MINGW32_MAJOR_VERSION * 100) + __MINGW32_MINOR_VERSION * 10
#elif defined(__BORLANDC__)
#	define AURORA_COMPILER				AURORA_COMPILER_BORLAND
#	define AURORA_COMPILER_VERSION		__BORLANDC__
#	define AURORA_ASSEMBLY_SYNTAX		AURORA_SYNTAX_UNAVAILABLE
#elif defined(__INTEL_COMPILER)
#	define AURORA_COMPILER				AURORA_COMPILER_INTEL
#	define AURORA_COMPILER_VERSION		__INTEL_COMPILER
#	define AURORA_ASSEMBLY_SYNTAX		AURORA_SYNTAX_INTEL
//  ATTENTION IF YOU REACH THIS PREPROCESSOR BRANCH! Aurora has detected an Intel compiler. Please compile using -use-msasm option if you want to use inline assembly (or disable it in AuroraConfig.h)
#else
#	define AURORA_COMPILER				AURORA_COMPILER_UKNOWN
#	define AURORA_ASSEMBLY_SYNTAX		AURORA_SYNTAX_UNAVAILABLE
#	error("Unknown compiler! Aurora cannot be safely compiled. If you know this is a mistake, please report a bug and/or comment this error line.")
#endif

#if AURORA_ARCHITECTURE == AURORA_ARCHITECTURE_X64 && AURORA_COMPILER == AURORA_COMPILER_MSVC
// Microsoft removed support for inline assembly on 64bit systems. Time to move to another compiler.
#define AURORA_ASSEMBLY_AVAILABLE		false
#else
#define AURORA_ASSEMBLY_AVAILABLE		(AURORA_ASSEMBLY_SYNTAX != AURORA_SYNTAX_UNAVAILABLE)
#endif

// Detect the operating system
#if defined(_WIN32) || defined(WIN32)
#	define AURORA_OS					AURORA_OS_WINDOWS
#elif defined(__linux__) || defined(linux) || defined(__linux) || defined(__unix) || defined(unix)
#	define AURORA_OS					AURORA_OS_LINUX // treat unix as linux (diiiiirty)
#elif defined(__APPLE__) && defined(__MACH__)
#	define AURORA_OS					AURORA_OS_OSX
#else
#	error ("Unsupported OS! Aurora cannot be safely compiled. If you know this is a mistake, please report a bug and/or comment this error line.")
#endif

// Detect the underlying processor architecture
#if defined(__amd64__) || defined(__x86_64) || defined(_M_X64) || defined(__ia64__) || defined(_M_IA64)
#	define AURORA_ARCHITECTURE			AURORA_ARCHITECUTRE_X64
#elif defined(i386) || defined(__i386__) || defined (_X86_) || defined(_M_IX86)
#	define AURORA_ARCHITECTURE			AURORA_ARCHITECTURE_X86
#else
#	define AURORA_ARCHITECTURE			AURORA_ARCHITECTURE_UNKNOWN
#	error("Unknown architecture! Aurora cannot be safely compiled. If you know this is a mistake, please report a bug and/or comment this error line.")
#endif

#if AURORA_COMPILER == AURORA_COMPILER_MSVC
#	if defined(AURORA_SOURCE_BUILD)
#		define AURORA_LIBRARY			__declspec(dllexport)
#	else
#		define AURORA_LIBRARY			__declspec(dllimport)
#	endif
#else
	// On GCC, there are visibility attributes, but everything's public by default.
#	define AURORA_LIBRARY
#endif

#if defined(AURORA_DEBUG) && AURORA_DEBUG_DISABLE_SSE == 1
#	define AURORA_SSE_ENABLED				(false)
#else
#	define AURORA_SSE_ENABLED				(AURORA_USE_INLINE_ASSEMBLY && AURORA_ASSEMBLY_AVAILABLE && !AURORA_USE_DOUBLE_PRECISION && AURORA_USE_SSE && (AURORA_COMPILER == AURORA_COMPILER_MSVC || AURORA_COMPILER == AURORA_COMPILER_GCC || AURORA_COMPILER == AURORA_COMPILER_MINGW || AURORA_COMPILER == AURORA_COMPILER_INTEL))
#endif

#ifdef AURORA_DEBUG
#	if AURORA_TRACK_MEMORY == 0
#		define AURORA_MEMORY_TRACKING			1
#	endif
#else
#	if AURORA_TRACK_MEMORY == 0
#		define AURORA_MEMORY_TRACKING			0
#	else
#		define AURORA_MEMORY_TRACKING			(AURORA_TRACK_MEMORY==1? 0 : 1)
#	endif
#endif

#define AURORA_SSE_ALIGNMENT_SIZE			16 // 16-byte aligned

#if AURORA_SSE_ENABLED
#	if AURORA_COMPILER == AURORA_COMPILER_GCC || AURORA_COMPILER == AURORA_COMPILER_MINGW
#		define AURORA_SSE_ALIGN				__attribute__((aligned(AURORA_SSE_ALIGNMENT_SIZE)))
#	elif AURORA_COMPILER == AURORA_COMPILER_MSVC
#		define AURORA_SSE_ALIGN				__declspec(align(AURORA_SSE_ALIGNMENT_SIZE))
#	elif AURORA_COMPILER == AURORA_COMPILER_INTEL
#		define AURORA_SSE_ALIGN				__declspec(align(AURORA_SSE_ALIGNMENT_SIZE))
#	else
#		define AURORA_SSE_ALIGN
#	endif
#else
#	define AURORA_SSE_ALIGN
#endif

#if AURORA_ARCHITECTURE == AURORA_ARCHITECTURE_X86
#define AURORA_DEFAULT_ALIGNMENT			8
#elif AURORA_ARCHITECTURE == AURORA_ARCHITECTURE_X64
#define AURORA_DEFAULT_ALIGNMENT			16
#endif

#if AURORA_USE_DOUBLE_PRECISION == 0
#define AURORA_FPTOLERANCE				1e-06
#else
#define AURORA_FPTOLERANCE				1e-15
#endif

#endif // __AURORA_PLATFORM_H__
