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
#include "AuroraScene.h"
#include "AuroraSceneNode.h"
#include "AuroraException.h"

using namespace Aurora;

Scene::Scene()
{
	mRootNode = createSceneNode("Root", NULL);
	mRootNode->setIsRoot(true);
}

SceneNode* Scene::createSceneNode(String Name, SceneNode* Parent)
{
	SceneNode *p;
#	if AURORA_CACHING_LEVEL >= 1
	if (!mFreeSlots.empty())
	{
		// Take the last slot
		p = mFreeSlots.back();
		AURORA_PLACEMENT_NEW(p) SceneNode(Name, this, Parent);
		mFreeSlots.pop_back();
	}
	else
	{
		p = AURORA_NEW SceneNode(Name, this, Parent);
	}
#	else
	p = AURORA_NEW SceneNode(Name, this, Parent);
#	endif

	mCreatedNodes.insert(p);
	return p;
}

void Scene::destroySceneNode(SceneNode* ToDestroy)
{
	NodeSetIterator it = mCreatedNodes.find(ToDestroy);
	if (it == mCreatedNodes.end())
		throw NonExistentException();

#	if AURORA_CACHING_LEVEL >= 1
	uint numSlots = mFreeSlots.size();
	if (numSlots < AURORA_SCENE_MAX_FREE_SLOTS)
	{
		ToDestroy->~SceneNode(); // ugly, I know

		// even more ugly:
		mFreeSlots.push_back(ToDestroy); // Save this but don't deallocate
	}
	else
	{
		AURORA_DELETE ToDestroy;
	}
#	else
	AURORA_DELETE ToDestroy;
#	endif

	mCreatedNodes.erase(it);
}

Scene::~Scene()
{
	for (NodeSetIterator it = mCreatedNodes.begin(); it != mCreatedNodes.end(); ++it)
		AURORA_DELETE *it;

#	if AURORA_CACHING_LEVEL >= 1
	for (FreeSlotListIterator it = mFreeSlots.begin(); it != mFreeSlots.end(); ++it)
		AURORA_DELETE *it;
#	endif
}

