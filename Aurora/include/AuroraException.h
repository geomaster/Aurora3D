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
#ifndef __AURORA_EXCEPTION_H__
#define __AURORA_EXCEPTION_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include <exception>

#define AURORA_DEFINE_EXCEPTION(name, description) \
    class name : public Exception \
    { \
    public: \
        virtual const char* what() const throw() \
        { \
            return description; \
        } \
        \
        virtual ~name() throw() \
        { \
        \
        } \
    }

#define AURORA_DEFINE_EXCEPTION_INHERIT(name, inheritfrom, description) \
    class name : public inheritfrom \
    { \
	protected: \
		Aurora::String mDetails; \
    public: \
		name() : mDetails("No details provided.") \
		{ \
		\
		} \
		\
		name(String Details) : mDetails(Details) \
		{ \
		\
		} \
		\
        const char* what() const throw() \
        { \
            return (String(description) + String("\nDetails:\n") + mDetails).c_str(); \
        } \
        \
        virtual ~name() throw() \
        { \
        \
        } \
    }

#define AURORA_THROW_EXCEPTION(type, description, where) \
	throw type (Aurora::String(description) + Aurora::String("\nat ") + Aurora::String(where))

namespace Aurora
{
    class AURORA_LIBRARY Exception : public std::exception, virtual public Alloc
    {
    public:
        Exception() throw()
        {
        }

        Exception(const Exception& other) throw()
        {
        }

        virtual const char* what() const throw()
        {
            return "Generic Aurora exception";
        }

        virtual ~Exception() throw()
        {
        }
    };

    AURORA_DEFINE_EXCEPTION(UnimplementedException, "An unimplemented feature has been requested.");
    AURORA_DEFINE_EXCEPTION(IllegalOperationException, "Attempt to execute an operation not applicable to current state.");
    AURORA_DEFINE_EXCEPTION(InternalErrorException, "An internal error has occurred inside Aurora.");
    // TODO: add more exceptions if needed

	AURORA_DEFINE_EXCEPTION_INHERIT(NonExistentException, IllegalOperationException, "An unrecognized object was required.");
	AURORA_DEFINE_EXCEPTION_INHERIT(NonExistentNameException, NonExistentException, "An object of an unrecognized name was required.");
	AURORA_DEFINE_EXCEPTION_INHERIT(DuplicateParentException, IllegalOperationException, "An object is already a child of another object.");
}

#endif // __AURORA_EXCEPTION_H__
