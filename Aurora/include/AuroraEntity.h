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
#ifndef __AURORA_ENTITY_H__
#define __AURORA_ENTITY_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"

namespace Aurora
{
	class Entity : virtual public Alloc
	{
	protected:
		SceneNode *mParent;

	public:
		Entity(SceneNode *Parent) : mParent(Parent) { ; }

		virtual void onAttached(SceneNode *NewParent)
		{
			mParent = NewParent;
		}

		virtual void onDetached()
		{
			mParent = NULL;
		}

		virtual SceneNode* getParent()
		{
			return mParent;
		}

		virtual void update() = 0;

		virtual ~Entity() { ; }
	};
}

#endif // __AURORA_ENTITY_H__
