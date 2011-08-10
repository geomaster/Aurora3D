#ifndef __AURORA_HASHMAP_H__
#define __AURORA_HASHMAP_H__
#include "../AuroraPrereqs.h"
#include "../AuroraMemoryConfig.h"

#include "Base.h"
#include <boost/unordered_map.hpp>

namespace Aurora
{
	namespace STL
	{
		template<typename K, typename V>
		struct HashMap
		{
			typedef boost::unordered_map<K, V> default_type;
			typedef typename default_type::value_type value_type;

			typedef boost::unordered_map<K, V, Aurora::Hash<K>, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::iterator>::Forward iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantForward const_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::Reverse reverse_iterator;
			typedef typename IteratorWrappers<value_type, typename type::iterator>::ConstantReverse const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};

		template<typename K, typename V>
		struct ApproximateHashMap
		{
			typedef boost::unordered_map<K, V> default_type;
			typedef typename default_type::value_type value_type;

			typedef boost::unordered_map<K, V, Aurora::ApproximateHash<K>, typename STLAllocators<value_type>::Default> type;

			typedef IteratorWrappers<value_type>::Forward iterator;
			typedef IteratorWrappers<value_type>::ConstantForward const_iterator;
			typedef IteratorWrappers<value_type>::Reverse reverse_iterator;
			typedef IteratorWrappers<value_type>::ConstantReverse const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};
	}
}
#endif // __AURORA_HASHMAP_H__
