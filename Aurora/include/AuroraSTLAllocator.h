#ifndef __AURORA_STL_ALLOCATOR_H__
#define __AURORA_STL_ALLOCATOR_H__
#include "AuroraPrereqs.h"

namespace Aurora
{
	template< typename T, typename Allocator >
	class STLAllocator
	{
	public:
		typedef T value_type;
		typedef value_type*	pointer;
		typedef const value_type* const_pointer;
		typedef value_type&	reference;
		typedef const value_type& const_reference;
		typedef std::size_t	size_type;
		typedef std::ptrdiff_t difference_type;

		template< typename OtherType >
 		struct rebind
		{
			typedef STLAllocator< OtherType, Allocator > other;
		};

		inline STLAllocator() { ; }
		inline STLAllocator(const STLAllocator<T, Allocator>&) { ; }
		template< typename oT, typename oAllocator >
		inline STLAllocator(const STLAllocator<oT, oAllocator>&) { ; }
		virtual ~STLAllocator() { ; }

		inline pointer allocate(size_type Count, std::allocator<void>::const_pointer ptr = NULL)
		{
			return static_cast<pointer>(Allocator::allocateMemory(Count * sizeof(value_type)));
		}

		inline void deallocate(pointer Mem, size_type Sz = 0)
		{
			Allocator::deallocateMemory(static_cast<void*>( Mem ));
		}

		inline void construct(pointer Mem, const T& ConstructFrom)
		{
			new(static_cast<void*>( Mem )) T(ConstructFrom);
		}

		inline void destroy(pointer Mem)
		{
			Mem->~T();
		}

		inline size_type max_size() const
		{
			return Allocator::getMaxSize();
		}

		inline pointer address(reference X) const
		{
			return &X;
		}

		inline const_pointer address(const_reference X) const
		{
			return &X;
		}
	};

	template<typename T, typename Allocator, typename oT>
	inline AURORA_LIBRARY bool operator==(STLAllocator<T, Allocator> One, STLAllocator<oT, Allocator> Two)
	{
		// yes, we have the same allocator here
		return true;
	}

	template<typename T, typename Allocator, typename OtherType>
	inline AURORA_LIBRARY bool operator==(STLAllocator<T, Allocator> One, OtherType Two)
	{
		// no! not same types.
		return false;
	}

	template<typename T, typename Allocator, typename oT>
	inline AURORA_LIBRARY bool operator!=(STLAllocator<T, Allocator> One, STLAllocator<oT, Allocator> Two)
	{
		// they're equal
		return false;
	}

	template<typename T, typename Allocator, typename OtherType>
	inline AURORA_LIBRARY bool operator!=(STLAllocator<T, Allocator> One, OtherType Two)
	{
		// yes, they are indeed not equal.
		return true;
	}
}

#endif // __AURORA_STL_ALLOCATOR_H__
