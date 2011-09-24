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
#ifndef __AURORA_FACTORY_H__
#define __AURORA_FACTORY_H__
#include "AuroraPrereqs.h"

namespace Aurora
{
	template<typename T>
	class Factory
	{
	public:
		virtual String getType() const = 0;
		virtual T* create() = 0;
		virtual void destroy(T*) = 0;

		virtual ~Factory() { ; }
	};
}
#endif // __AURORA_FACTORY_H__