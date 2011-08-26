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
	if (mParent)
		_updateAbsoluteTransform(mParent->getAbsoluteTransform(), false);
	else mAbsoluteTransform = mTransform;

	return mAbsoluteTransform;
}

void SceneNode::translate(const Vector3D& Translation, TransformSpace RelativeTo)
{
	Vector3D transl;
	Transform parentT;

	switch(RelativeTo)
	{
	case ETS_Parent:
		break;

	case ETS_Local:
		transl = mTransform.Orientation * Translation;
		break;

	case ETS_World:
		if (mParent)
		{
			parentT = mParent->getAbsoluteTransform();
			// Negate the global rotation, apply the translation, take into account
			// parent's scale
			transl = (parentT.Orientation.getInverse() * Translation) / parentT.Scale;
		}
		break;

	default:
		break;
	}

	mTransform.Position += transl;
	_notifyChildrenNeedUpdate();
	_notifyNeedsUpdate();
}

void SceneNode::rotate(const Quaternion& Rotation, TransformSpace RelativeTo)
{
	Transform t;
	switch(RelativeTo)
	{
	case ETS_Parent:
		// Note that we are applying the given rotation first and then the previous
		// one (default)
		mTransform.Orientation = Rotation * mTransform.Orientation;
		break;

	case ETS_Local:
		// Note that we just concatenate the rotation because of the local transform space
		mTransform.Orientation *= Rotation;
		break;

	case ETS_World:
		t = getAbsoluteTransform();
		// q^-1 p q
		// First we negate the absolute transform, apply the given world transform,
		// then put back into our coordinate frame by transforming again
		mTransform.Orientation *= t.Orientation.getInverse() * Rotation * t.Orientation;
		break;

	default:
		break;
	}

	_notifyChildrenNeedUpdate();
	_notifyNeedsUpdate();
}

Transform SceneNode::_updateAbsoluteTransform(const Transform& ParentTransform, bool Propagate, bool Force)
{
	if (Force || mNeedsUpdate)
	{
		Transform thisTransform = ParentTransform.concatenate(mTransform);
		mAbsoluteTransform = thisTransform;

		if (Propagate)
		{
			for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
				it->second->_updateAbsoluteTransform(thisTransform, true, true);
		}

		mNeedsUpdate = false;

		//i apparently had a logic collapse here, how can possibly children
		//be uninformed when we recompute the update?
		/*if (!Propagate && InformChildren)
			_notifyChildrenNeedUpdate();*/

		return thisTransform;
	}
	else return mAbsoluteTransform;
}

SceneNode* SceneNode::createChildSceneNode(String Name, const Transform& ChildTransform)
{
	SceneNode* n;
	n = mScene->createSceneNode(Name, NULL);
	n->setTransform(ChildTransform);
	addChild(n);

	return n;
}

SceneNode* SceneNode::createChildSceneNode(String Name, const Vector3D& Position,
				 const Quaternion& Orientation,
				 const Vector3D& Scale)
{
	return createChildSceneNode(Name, Transform(Position, Orientation, Scale));
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
		throw DuplicateParentException("The Entity provided is already a child of another SceneNode");

	NewEntity->onAttached(this);
	mEntities.insert(NewEntity);

}

void SceneNode::detachEntity(Entity* OldEntity)
{
	EntityListIterator it = mEntities.find(OldEntity);
	if (it == mEntities.end())
		throw NonExistentException();

	OldEntity->onDetached();
	mEntities.erase(it);
}

SceneNode::~SceneNode()
{
	if (mParent) mParent->removeChild(mName);

	for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
		it->second->setParent(NULL);
}
