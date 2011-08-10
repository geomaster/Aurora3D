#ifndef __AURORA_VECTOR_H__
#define __AURORA_VECTOR_H__
#include "../AuroraPrereqs.h"
#include "../AuroraMemoryConfig.h"
#include "Base.h"
#include <vector>

namespace Aurora
{
	namespace STL
	{
		template<typename T>
		struct Vector
		{
			typedef std::vector<T> default_type;
			typedef typename default_type::value_type value_type;

			typedef std::vector<T, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::iterator>::Forward iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantForward const_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::Reverse reverse_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantReverse const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};
	}
}
#endif // __AURORA_VECTOR_H__
