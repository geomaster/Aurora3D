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
#include "AuroraNode.h"
using namespace Aurora;

void Node::addChild(Node* Child)
{
	AURORA_ASSERT(!Child->getParent() && "This node already has a parent.");
	mChildren.insert(ChildrenMap::value_type(Child->getName(), Child));
	Child->setParent(this);
}

void Node::removeChild(Node* Child)
{
	removeChild(Child->getName());
}

void Node::removeChild(String ChildName)
{
	ChildrenMapIterator it = mChildren.find(ChildName);
	AURORA_ASSERT(it != mChildren.end() "The node supplied is not a child of this node.");

	it->second->_notifyDetached();
	mChildren.erase(it);
}

Node* Node::getChildByName(String Name) const
{
	ChildrenMapConstIterator it = mChildren.find(Name);
	if (it == mChildren.end())
	{
		throw NonExistentNameException();
	}
	else return it->second;
}

Transform Node::getAbsoluteTransform()
{
	if (!mNeedsUpdate) return mAbsoluteTransform;

	_updateAbsoluteTransform(mParent->getAbsoluteTransform(), false);
	_notifyChildrenNeedUpdate();

	return mAbsoluteTransform;
}

void Node::translate(const Vector3D& Translation, TransformSpace RelativeTo)
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

void Node::rotate(const Quaternion& Rotation, TransformSpace RelativeTo)
{
	//TODO: Check if this all works fine!
	Quaternion rot = Rotation;
	switch(RelativeTo)
	{
	case ETS_Parent:
		break;

	case ETS_Local:
		AURORA_ASSERT(!"No idea about this. Check with OGRE code and remove this assertion when sure");
		break;

	case ETS_World:
		rot *= mParent->getAbsoluteTransform().Rotation.getInverse();
		break;

	default:
		break;
	}

	mTransform.Rotation *= rot;
}

void Node::scale(const Vector3D& Scale, TransformSpace RelativeTo)
{
	//TODO: Check if this all works fine!
	Vector3D scale = Scale;
	switch(RelativeTo)
	{
	case ETS_Parent:
		break;

	case ETS_Local:
		AURORA_ASSERT(!"No idea about this. Check with OGRE code and remove this assertion when sure");
		break;

	case ETS_World:
		scale /= mParent->getAbsoluteTransform().Scale;
		break;

	default:
		break;
	}

	mTransform.Scale *= scale;
}

Transform Node::_updateAbsoluteTransform(const Transform& ParentTransform, bool UpdateChildren)
{
	Transform thisTransform = ParentTransform.concatenate(mTransform);
	mTransform = thisTransform;

	if (UpdateChildren)
	{
		for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
			it->second->_updateAbsoluteTransform(thisTransform, true);
	}

	mNeedsUpdate = false;
	return thisTransform;
}

Node::~Node()
{
	for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
		it->second->setParent(NULL);
}
