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
#ifndef __AURORA_HASHMAP_H__
#define __AURORA_HASHMAP_H__
#include "../AuroraPrereqs.h"
#include "../AuroraMemoryConfig.h"

#include "Base.h"
#include <boost/unordered_map.hpp>
#include "../AuroraHash.h"

namespace Aurora
{
	namespace STL
	{
		template<typename K, typename V, typename EqualComp = std::equal_to<K> >
		struct HashMap
		{
			typedef boost::unordered_map<K, V, EqualComp> default_type;
			typedef typename default_type::value_type value_type;

			typedef boost::unordered_map<K, V, AURORA_HASH(K), EqualComp, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::const_iterator>::ConstantForward const_iterator;

			typedef typename IteratorWrappers<V, typename type::iterator>::ValueForward value_iterator;
			typedef typename IteratorWrappers<V, typename type::const_iterator>::ValueConstantForward value_const_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};

		template<typename K, typename V, typename EqualComp = std::equal_to<K> >
		struct ApproximateHashMap
		{
			typedef boost::unordered_map<K, V, EqualComp> default_type;
			typedef typename default_type::value_type value_type;

			typedef boost::unordered_map<K, V, AURORA_APPROX_HASH(K), EqualComp, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::iterator>::Forward iterator;
			typedef typename IteratorWrappers<value_type, typename type::const_iterator>::ConstantForward const_iterator;

			typedef typename IteratorWrappers<V, typename type::iterator>::ValueForward value_iterator;
			typedef typename IteratorWrappers<V, typename type::const_iterator>::ValueConstantForward value_const_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};
	}
}
#endif // __AURORA_HASHMAP_H__
