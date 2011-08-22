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
#ifndef __AURORA_SCENE_H__
#define __AURORA_SCENE_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include <STL/HashSet.h>
#include <STL/Vector.h>

#define AURORA_SCENE_MAX_FREE_SLOTS				10

namespace Aurora
{
	// On caching level 1, the scene reuses some pointers and doesn't deallocate them
	// right away in order to avoid expensive allocation/deallocation.

	class Scene : virtual public Alloc
	{
	protected:
		typedef STL::HashSet<SceneNode*>::type NodeSet;
		typedef NodeSet::iterator NodeSetIterator;
		typedef NodeSet::const_iterator NodeSetConstIterator;

		typedef STL::Vector<SceneNode*>::type FreeSlotList;
		typedef FreeSlotList::iterator FreeSlotListIterator;
		typedef FreeSlotList::const_iterator FreeSlotListConstIterator;

		SceneNode* mRootNode;
		SceneManager* mSceneManager;
		NodeSet mCreatedNodes;

#		if AURORA_CACHING_LEVEL >= 1
		FreeSlotList mFreeSlots;
#		endif

	public:
		Scene();

		virtual SceneNode* createSceneNode(String Name, SceneNode* Parent = NULL);
		virtual void destroySceneNode(SceneNode* ToDestroy);

		virtual void setSceneManager(SceneManager* NewMgr)
		{
			mSceneManager = NewMgr;
		}

		virtual SceneManager* getSceneManager()
		{
			return mSceneManager;
		}

		virtual SceneNode* getRootSceneNode()
		{
			return mRootNode;
		}

		virtual ~Scene();
	};
};

#endif // __AURORA_SCENE_H__
