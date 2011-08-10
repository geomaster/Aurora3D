#ifndef __AURORA_HASHSET_H__
#define __AURORA_HASHSET_H__
#include "../AuroraPrereqs.h"
#include "../AuroraMemoryConfig.h"

#include "Base.h"
#include <boost/unordered_set.hpp>

namespace Aurora
{
	namespace STL
	{
		template<typename K>
		struct HashSet
		{
			typedef boost::unordered_set<K> default_type;
			typedef typename default_type::value_type value_type;

			typedef boost::unordered_set<K, Aurora::Hash<K>, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::iterator>::Forward iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantForward const_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::Reverse reverse_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantReverse const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};

		template<typename K>
		struct ApproximateHashSet
		{
			typedef boost::unordered_set<K> default_type;
			typedef typename default_type::value_type value_type;

			typedef boost::unordered_set<K, Aurora::ApproximateHash<K>, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::iterator>::Forward iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantForward const_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::Reverse reverse_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantReverse const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};
	}
}
#endif // __AURORA_HASHSET_H__
