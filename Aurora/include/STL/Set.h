#ifndef __AURORA_SET_H__
#define __AURORA_SET_H__
#include "../AuroraPrereqs.h"
#include "../AuroraMemoryConfig.h"

#include "Base.h"
#include <set>

namespace Aurora
{
	namespace STL
	{
		template<typename K, typename Comp = std::less<K> >
		struct Set
		{
			typedef std::set<K, Comp> default_type;
			typedef typename default_type::value_type value_type;

			typedef std::set<K, Comp, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::iterator>::Forward iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantForward const_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::Reverse reverse_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantReverse const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};
	}
}
#endif // __AURORA_SET_H__
