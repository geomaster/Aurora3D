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
#include "AuroraPipeline.h"
#include "AuroraEntity.h"
#include "AuroraSceneNode.h"
#include "AuroraException.h"

using namespace Aurora;

SceneManager::SceneManager() : mScene(NULL)
{

}

void SceneManager::addPipeline(Pipeline *P)
{
	mPipelines.push_back(P);
	EntityList l;
	mEntities.insert(make_pair(P, l));
}

void SceneManager::removePipeline(Pipeline* ToRemove)
{
	PipelineListIterator it;
	for (it = mPipelines.begin(); it != mPipelines.end(); ++it)
	{
		if (*it == ToRemove)
			break;
	}

	if (it == mPipelines.end())
		throw NonExistentException("The pipeline provided is not registered with this SceneManager");

	PipelineEntityListIterator it2 = mEntities.find(ToRemove);
	AURORA_ASSERT(it2 != mEntities.end(), "INTERNAL ERROR!");

	mPipelines.erase(it);
	mEntities.erase(it2);
}

void SceneManager::_registerEntity(Entity* Ent)
{
	Pipeline* own = Ent->getPipeline();
	PipelineEntityListIterator it = mEntities.find(own);

	if (it == mEntities.end())
		throw NonExistentException("The parent of the entity provided is not registered with this SceneManager");

	AURORA_ASSERT(own->getType() == EPT_Immediate, "Only immediate pipelines should register entities");

	//FIXME: Unsafe base->derived cast
	it->second.insert(EntitySpec(Ent, static_cast<ImmediatePipeline*>(own)));
}

void SceneManager::_unregisterEntity(Entity* Ent)
{
	PipelineEntityListIterator it = mEntities.find(Ent->getPipeline());
	if (it == mEntities.end())
		throw NonExistentException("The parent of the entity provided is not registered with this SceneManager");

	EntityListIterator it2 = it->second.find(Ent);
	if (it2 == it->second.end())
		throw NonExistentException("The Entity provided is not registered with this SceneManager");

	it->second.erase(it2);
}

void SceneManager::update()
{
	// Update the scene graph
	mScene->getRootSceneNode()->_updateAbsoluteTransform(Transform(), true);

	for (PipelineListIterator it = mPipelines.begin(); it != mPipelines.end(); ++it)
	{
		if ((*it)->getType() == EPT_Immediate)
		{
			PipelineEntityListIterator li = mEntities.find(*it);
			EntityList* l = &li->second;

			//FIXME: Unsafe cast
			ImmediatePipeline* p = static_cast<ImmediatePipeline*>(*it);
			p->reserveEntitySpace(l->size());
			for (EntityListIterator it2 = l->begin(); it2 != l->end(); ++it2)
				p->queueEntity(it2->Ent);
		}
		(*it)->update();
	}
}

void SceneManager::registerEntities(SceneNode* From)
{
	SceneNode::EntityConstIteratorWrapper it = From->getEntityIterator();
	while (it.hasMoreElements())
	{
		if (it.get()->getPipeline()->getType() == EPT_Immediate)
			_registerEntity(it.get());

		it.increment();
	}

	SceneNode::ChildrenConstIteratorWrapper it2 = From->getChildrenIterator();
	while (it2.hasMoreElements())
	{
		registerEntities(it2.get());
		it2.increment();
	}
}

void SceneManager::setScene(Scene* NewScene)
{
	for (PipelineEntityListIterator it = mEntities.begin(); it != mEntities.end(); ++it)
		it->second.clear();

	NewScene->setSceneManager(this);
	registerEntities(NewScene->getRootSceneNode());
	mScene = NewScene;
}
