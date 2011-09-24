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
#ifndef __AURORA_MAP_H__
#define __AURORA_MAP_H__
#include "../AuroraPrereqs.h"
#include "../AuroraMemoryConfig.h"

#include "Base.h"
#include <map>

namespace Aurora
{
	namespace STL
	{
		template<typename K, typename V, typename Comp = std::less<K> >
		struct Map
		{
			typedef std::map<K, V, Comp> default_type;
			typedef typename default_type::value_type value_type;

			typedef std::map<K, V, Comp, typename STLAllocators<value_type>::Default> type;

			typedef typename IteratorWrappers<value_type, typename type::const_iterator>::ConstantForward const_iterator;
			typedef typename IteratorWrappers<value_type, typename type::const_reverse_iterator>::ConstantReverse const_reverse_iterator;

			typedef typename IteratorWrappers<V, typename type::iterator>::ValueForward value_iterator;
			typedef typename IteratorWrappers<V, typename type::const_iterator>::ValueConstantForward value_const_iterator;
			typedef typename IteratorWrappers<V, typename type::reverse_iterator>::ValueReverse value_reverse_iterator;
			typedef typename IteratorWrappers<V, typename type::const_reverse_iterator>::ValueConstantReverse value_const_reverse_iterator;

//			typedef ContainerWrapper<value_type, type> container;
		};
	}
}
#endif // __AURORA_MAP_H__
