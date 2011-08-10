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
#include <STL/Vector.h>

namespace Aurora
{
	class Scene : virtual public Alloc
	{
	protected:
		SceneNode* mRootNode;
		SceneManager* mSceneManager;
		STL::Vector<SceneNode*>::type mCreatedNodes;

	public:
		Scene();

		virtual void createSceneNode();
		virtual void destroySceneNode(SceneNode *ToDestroy);

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
	};
};

#endif // __AURORA_SCENE_H__
