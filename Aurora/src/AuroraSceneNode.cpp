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
#include "AuroraSceneNode.h"
#include "AuroraScene.h"
#include "AuroraSceneManager.h"
#include "AuroraEntity.h"
#include "AuroraException.h"

using namespace Aurora;

void SceneNode::addChild(SceneNode* Child)
{
	if (Child->getParent())
		throw DuplicateParentException();

	mChildren.insert(ChildrenMap::value_type(Child->getName(), Child));
	Child->setParent(this);
}

void SceneNode::removeChild(SceneNode* Child)
{
	removeChild(Child->getName());
}

void SceneNode::removeChild(String ChildName)
{
	ChildrenMapIterator it = mChildren.find(ChildName);
	AURORA_ASSERT(it != mChildren.end(), "The node supplied is not a child of this node.");

	it->second->_notifyDetached();
	mChildren.erase(it);
}

SceneNode* SceneNode::getChildByName(String Name) const
{
	ChildrenMapConstIterator it = mChildren.find(Name);
	if (it == mChildren.end())
	{
		throw NonExistentNameException();
	}
	else return it->second;
}

Transform SceneNode::getAbsoluteTransform()
{
	if (!mNeedsUpdate) return mAbsoluteTransform;
	_updateAbsoluteTransform(mParent->getAbsoluteTransform(), false);

	return mAbsoluteTransform;
}

void SceneNode::translate(const Vector3D& Translation, TransformSpace RelativeTo)
{
	//TODO: Check if this all works fine!
	Vector3D transl = Translation;
	switch(RelativeTo)
	{
	case ETS_Parent:
		break;

	case ETS_Local:
		transl = mTransform.Rotation * transl;
		transl *= mTransform.Scale;
		break;

	case ETS_World:
		transl -= mParent->getAbsoluteTransform().Translation;
		break;

	default:
		break;
	}

	mTransform.Translation += transl;
}

void SceneNode::rotate(const Quaternion& Rotation, TransformSpace RelativeTo)
{
	//TODO: Check if this all works fine!
	Quaternion rot = Rotation;
	switch(RelativeTo)
	{
	case ETS_Parent:
		break;

	case ETS_Local:
		AURORA_ASSERT(false, "No idea about this. Check with OGRE code and remove this assertion when sure");
		break;

	case ETS_World:
		rot *= mParent->getAbsoluteTransform().Rotation.getInverse();
		break;

	default:
		break;
	}

	mTransform.Rotation *= rot;
}

void SceneNode::scale(const Vector3D& Scale, TransformSpace RelativeTo)
{
	//TODO: Check if this all works fine!
	Vector3D scale = Scale;
	switch(RelativeTo)
	{
	case ETS_Parent:
		break;

	case ETS_Local:
		AURORA_ASSERT(false, "No idea about this. Check with OGRE code and remove this assertion when sure");
		break;

	case ETS_World:
		scale /= mParent->getAbsoluteTransform().Scale;
		break;

	default:
		break;
	}

	mTransform.Scale *= scale;
}

Transform SceneNode::_updateAbsoluteTransform(const Transform& ParentTransform, bool UpdateChildren)
{
	Transform thisTransform = ParentTransform.concatenate(mTransform);
	mAbsoluteTransform = thisTransform;

	if (UpdateChildren)
	{
		for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
			it->second->_updateAbsoluteTransform(thisTransform, true);
	}

	mNeedsUpdate = false;
	if (!UpdateChildren)
		_notifyChildrenNeedUpdate();

	return thisTransform;
}

SceneNode* SceneNode::createChildSceneNode(String Name, const Transform& ChildTransform)
{
	SceneNode* n;
	n = mScene->createSceneNode(Name, NULL);
	n->setTransform(ChildTransform);
	addChild(n);

	return n;
}

SceneNode* SceneNode::createChildSceneNode(String Name, const Vector3D& Translation,
				 const Quaternion& Rotation,
				 const Vector3D& Scale)
{
	return createChildSceneNode(Name, Transform(Translation, Rotation, Scale));
}

void SceneNode::removeAndDestroyChild(SceneNode *Child)
{
	removeAndDestroyChild(Child->getName());
}

void SceneNode::removeAndDestroyChild(String ChildName)
{
	ChildrenMapIterator it = mChildren.find(ChildName);
	AURORA_ASSERT(it != mChildren.end(), "Node is not a children of this scene node");

	mScene->destroySceneNode(it->second);
	mChildren.erase(it);
}

void SceneNode::removeAndDestroyChildren()
{
	for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
		mScene->destroySceneNode(it->second);

	mChildren.clear();
}

void SceneNode::attachEntity(Entity* NewEntity)
{
	if (NewEntity->getParent())
		throw DuplicateParentException();

	NewEntity->onAttached(this);
	mEntities.insert(NewEntity);

	SceneManager* sm;
	if ((sm = mScene->getSceneManager()) && (NewEntity->getPipeline()->getType() == EPT_Immediate))
		mScene->getSceneManager()->_registerEntity(NewEntity);
}

void SceneNode::detachEntity(Entity* OldEntity)
{
	EntityListIterator it = mEntities.find(OldEntity);
	if (it == mEntities.end())
		throw NonExistentException();

	OldEntity->onDetached();
	mEntities.erase(it);

	SceneManager* sm;
	if ((sm = mScene->getSceneManager()) && (OldEntity->getPipeline()->getType() == EPT_Immediate))
		mScene->getSceneManager()->_unregisterEntity(OldEntity);
}

SceneNode::~SceneNode()
{
	for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
		it->second->setParent(NULL);
}
