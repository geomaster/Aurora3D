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
#include "AuroraSceneManager.h"
#include "AuroraEntity.h"
#include "AuroraSceneNode.h"
#include "AuroraException.h"

using namespace Aurora;

SceneManager::SceneManager() : mScene(NULL)
{

}

void SceneManager::updateSceneGraph(SceneNode *Start, const Transform& ParentTransform, bool Force)
{
	bool DoForce = Force;

	// If this node needs update, we need to force the updates down from this node
	// because by setting InformChildren to false we are leaving their needsUpdate
	// flags probably wrong.
	if (!Force && Start->needsUpdate())
		DoForce = true;

	Transform t = Start->_updateAbsoluteTransform(ParentTransform, false, Force);

	if (Start->getNumberEntities())
	{
		SceneNode::EntityConstIteratorWrapper it = Start->getEntityIterator();
		while (it.hasMoreElements())
		{
			it.get()->update();
			it.increment();
		}
	}
	
	if (Start->getNumberChildren())
	{
		SceneNode::ChildrenConstIteratorWrapper it = Start->getChildrenIterator();
		while (it.hasMoreElements())
		{
			SceneNode *node = it.get();
			node->_updateAbsoluteTransform(t, false, DoForce);
			updateSceneGraph(node);
			it.increment();
		}
	}
}

void SceneManager::update()
{
	// Update the scene graph
	updateSceneGraph(mScene->getRootSceneNode());
}


void SceneManager::setScene(Scene* NewScene)
{
	NewScene->setSceneManager(this);
	mScene = NewScene;
}
