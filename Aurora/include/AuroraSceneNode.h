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
#ifndef __AURORA_SCENE_NODE_H__
#define __AURORA_SCENE_NODE_H__
#include "AuroraPrereqs.h"
#include "AuroraTransform.h"
#include "STL/HashSet.h"
#include "STL/HashMap.h"

namespace Aurora
{
	class AURORA_LIBRARY SceneNode : virtual public SSEAlloc
	{
	public:
		typedef STL::HashSet<Entity*> EntityListSTL;
		typedef STL::HashMap<String, SceneNode*> ChildrenMapSTL;

		typedef ChildrenMapSTL::value_const_iterator ChildrenConstIteratorWrapper;
		typedef EntityListSTL::const_iterator EntityConstIteratorWrapper;

    private:
        SceneNode(const SceneNode& other)
        {
            AURORA_ASSERT(false, "No two nodes of same name should exist. Hence copying is impossible.");
        }

	protected:
		typedef ChildrenMapSTL::type ChildrenMap;
		typedef ChildrenMap::iterator ChildrenMapIterator;
		typedef ChildrenMap::const_iterator ChildrenMapConstIterator;

		typedef EntityListSTL::type EntityList;
		typedef EntityList::iterator EntityListIterator;
		typedef EntityList::const_iterator EntityListConstIterator;

		String mName;
		Transform mTransform;
		ChildrenMap mChildren;

		SceneNode *mParent;

        mutable Transform mAbsoluteTransform;
        mutable bool mNeedsUpdate;

        bool mIsRoot;

		Scene* mScene;
		EntityList mEntities;

	public:
		SceneNode(String Name, Scene* Owner, SceneNode* Parent = NULL) : mName(Name), mTransform(), mParent(Parent), mAbsoluteTransform(), mNeedsUpdate(true), mIsRoot(false), mScene(Owner)
		{
		}

		String getName() const
		{
			return mName;
		}

		virtual void addChild(SceneNode* Child);
		virtual void removeChild(SceneNode* Child);
		virtual void removeChild(String ChildName);
		virtual void removeAndDestroyChild(SceneNode *Child);
		virtual void removeAndDestroyChild(String ChildName);
        virtual void removeAndDestroyChildren();

		virtual SceneNode* getChildByName(String Name) const;
		virtual void setParent(SceneNode *Parent)
		{
			mParent = Parent;
		}

		virtual Transform getAbsoluteTransform();

		virtual Transform _updateAbsoluteTransform(const Transform& ParentTransform, bool Propagate = true, bool Force = false);
        virtual void _notifyNeedsUpdate()
        {
        	mNeedsUpdate = true;
        }

		virtual void _notifyChildrenNeedUpdate();

		virtual bool needsUpdate() const
		{
			return mNeedsUpdate;
		}
        virtual void _notifyDetached() { ; }
        virtual void _notifyAttached(SceneNode*) { ; }

        virtual void setIsRoot(bool IsRoot)
        {
            mIsRoot = IsRoot;
        }

        virtual bool isRoot()
        {
            return mIsRoot;
        }

		virtual SceneNode* getParent()
		{
		    return mParent;
		}

		const virtual SceneNode* getParent() const
		{
		    return mParent;
		}

		virtual Transform getTransform() const
		{
			return mTransform;
		}

		virtual Vector3D getPosition() const
		{
			return mTransform.Position;
		}

		virtual Quaternion getOrientation() const
		{
			return mTransform.Orientation;
		}

		virtual Vector3D getScale() const
		{
			return mTransform.Scale;
		}

		virtual void setPosition(const Vector3D& NewPosition);
		virtual void setOrientation(const Quaternion& NewOrientation);
		virtual void setScale(const Vector3D& NewScale);
		virtual void setTransform(const Transform& NewTransform);

		virtual void translate(const Vector3D& Translation, TransformSpace RelativeTo = ETS_Parent);
		virtual void rotate(const Quaternion& Rotation, TransformSpace RelativeTo = ETS_Parent);

		virtual SceneNode* createChildSceneNode(String Name, const Transform& ChildTransform = Transform());
		virtual SceneNode* createChildSceneNode(String Name, const Vector3D& Position,
						 const Quaternion& Orientation = Quaternion::Identity,
						 const Vector3D& Scale = Vector3D(Real( 1.0 )));

		virtual void attachEntity(Entity* NewEntity);
		virtual void detachEntity(Entity* OldEntity);

		virtual size_t getNumberEntities() const
		{
			return mEntities.size();
		}

		virtual size_t getNumberChildren() const
		{
			return mChildren.size();
		}

		virtual EntityConstIteratorWrapper getEntityIterator() const
		{
			return EntityConstIteratorWrapper(mEntities.cbegin(), mEntities.cbegin(), mEntities.cend());
		}

		virtual ChildrenConstIteratorWrapper getChildrenIterator() const
		{
			return ChildrenConstIteratorWrapper(mChildren.cbegin(), mChildren.cbegin(), mChildren.cend());
		}

		virtual ~SceneNode();
	};
}

#endif // __AURORA_SCENE_NODE_H__
