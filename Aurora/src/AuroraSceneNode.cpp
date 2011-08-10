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
using namespace Aurora;

Matrix4 Transform::toMatrix(bool IsQuaternionUnit) const
{
    Matrix4 mat = (IsQuaternionUnit? Rotation.toRotationMatrix4Unit() : Rotation.toRotationMatrix4());
    mat *= Matrix4::fromScale(Scale.getX(), Scale.getY(), Scale.getZ());

    mat.setEntry(0, 3, Translation.getX());
    mat.setEntry(1, 3, Translation.getY());
    mat.setEntry(2, 3, Translation.getZ());

    return mat;
}

void SceneNode::addChild(SceneNode *Child)
{
	String childName = Child->getName();
	if (mChildren.find(childName) != mChildren.end())
	{
        // TODO: throw an exception
	}
	else
	{
	    SceneNode* oldParent = Child->mParent;
	    oldParent->removeChild(Child);
	    Child->mParent = this;
	    mChildren.insert(std::make_pair(childName, Child));
	}
}

void SceneNode::removeChild(String ChildName)
{
    ChildrenMapIterator it = mChildren.find(ChildName);
    if (it != mChildren.end())
    {
        it->second->mParent = NULL;
        mChildren.erase(it);
    }
}

void SceneNode::removeChild(SceneNode* Child)
{
    removeChild(Child->getName());
}

 void SceneNode::removeAndDestroyChild(SceneNode *Child)
 {
    removeChild(Child->getName());
    delete Child;
 }

void SceneNode::removeAndDestroyChild(String ChildName)
{
    ChildrenMapIterator it = mChildren.find(ChildName);
    if (it != mChildren.end())
    {
        it->second->mParent = NULL;
        mChildren.erase(it);
        delete it->second;
    }
}

void SceneNode::removeAndDestroyChildren()
{
    for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
        delete it->second;

    mChildren.clear();
}

SceneNode* SceneNode::getChildByName(String Name)
{
    ChildrenMapIterator it = mChildren.find(Name);
    if (it != mChildren.end())
        return it->second;

    return NULL;
}

void SceneNode::setParent(SceneNode *Parent)
{
    Parent->addChild(this);
}

Transform SceneNode::getAbsoluteTransform() const
 {
     if (!mNeedsUpdate)
        return mAbsoluteTransform;

     if (mIsRoot)
     {
         mNeedsUpdate = false;
         mAbsoluteTransform = mTransform;

         return mAbsoluteTransform;
     }
     else
     {
         Transform parentTransform = mParent->getAbsoluteTransform();
         mAbsoluteTransform = parentTransform.concatenate(mTransform);
         mNeedsUpdate = false;

         return mAbsoluteTransform;
     }
}

void SceneNode::_setAbsoluteTransform(const Transform& ParentTransform, bool UpdateChildren)
{
    mAbsoluteTransform = ParentTransform.concatenate(mTransform);
    mNeedsUpdate = false;
    if (UpdateChildren && mChildren.size())
        for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
            it->second->_setAbsoluteTransform(mAbsoluteTransform, UpdateChildren);
}

void SceneNode::_notifyNeedsUpdate()
{
    mNeedsUpdate = false;
}

SceneNode* SceneNode::createChild(String Name, const Transform& ChildTransform)
{
    SceneNode *node = new SceneNode(Name, this);
    node->setTransform(ChildTransform);
    mChildren.insert(make_pair(Name, node));

    return node;
}

SceneNode* SceneNode::createChild(String Name, const Vector3D& Translation, const Quaternion& Rotation, const Vector3D& Scale)
{
    SceneNode *node = new SceneNode(Name, this);
    Transform t(Translation, Rotation, Scale);
    node->setTransform(t);
    mChildren.insert(make_pair(Name, node));

    return node;
}

SceneNode::~SceneNode()
{
    for (ChildrenMapIterator it = mChildren.begin(); it != mChildren.end(); ++it)
        delete it->second;
}
