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
            return Aurora::String(description) + Aurora::String("\n\nDetails:\n") + mDetails; \
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

	AURORA_DEFINE_EXCEPTION_INHERIT(DuplicateNameException, IllegalOperationException, "Attempt to add an already-existing node name into the scene graph.");
	AURORA_DEFINE_EXCEPTION_INHERIT(DuplicateParentException, IllegalOperationException, "Attempt to attach a node that is already a child of another node.");
}

#endif // __AURORA_EXCEPTION_H__
