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
#ifndef __AURORA_NODE_H__
#define __AURORA_NODE_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraVector3D.h"
#include "AuroraMatrix3.h"
#include "AuroraQuaternion.h"
#include "AuroraTransform.h"
#include "AuroraHash.h"
#include "STL/HashMap.h"
//#include "STL/Vector.h"

using namespace boost;
using namespace std;

#if AURORA_COMPILER == AURORA_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4251) // gets rid of annoying M$'s warning in VC++ regarding the fact that std::string isn't exported.
#endif

namespace Aurora
{
	class Scene;

	class AURORA_LIBRARY Node : virtual public SSEAlloc
	{
	public:
		typedef STL::HashMap<String, Node*>::type ChildrenMap;
		typedef ChildrenMap::iterator ChildrenMapIterator;
		typedef ChildrenMap::const_iterator ChildrenMapConstIterator;

    private:

        Node(const Node& other)
        {
            AURORA_ASSERT(!"No two nodes of same name should exist. Hence copying is impossible.");
        }

	protected:
		String mName;
		Transform mTransform;
		ChildrenMap mChildren;

		Node *mParent;

        mutable Transform mAbsoluteTransform;
        mutable bool mNeedsUpdate;
        bool mIsRoot;

	public:
/*
		Node() : mName(""), mTransform(), mParent(NULL), mNeedsUpdate(true), mIsRoot(false)
		{
		}
*/
		Node(String Name, Node *Parent = NULL) : mName(Name), mTransform(), mParent(Parent), mNeedsUpdate(true), mIsRoot(false)
		{
		}

		String getName() const
		{
			return mName;
		}

        virtual Node* createChild(String Name, const Transform& ChildTransform = Transform()) = 0;
        virtual Node* createChild(String Name, const Vector3D& Translation = Vector3D::Zero,
                         const Quaternion& Rotation = Quaternion::Identity,
                         const Vector3D& Scale = Vector3D(Real( 1.0 ))) = 0;

		virtual void addChild(Node* Child);
		virtual void removeChild(Node* Child);
		virtual void removeChild(String ChildName);
		virtual void removeAndDestroyChild(Node *Child) = 0;
		virtual void removeAndDestroyChild(String ChildName) = 0;
        virtual void removeAndDestroyChildren() = 0;

		virtual Node* getChildByName(String Name);
		virtual void setParent(Node *Parent)
		{
			mParent = Parent;
		}

		virtual Transform getAbsoluteTransform() const;
		virtual Transform _updateAbsoluteTransform(const Transform& ParentTransform, bool UpdateChildren);
        virtual void _notifyNeedsUpdate()
        {
        	mNeedsUpdate = true;
        }

        virtual void _notifyDetached() { ; }
        virtual void _notifyAttached() { ; }

        virtual void setIsRoot(bool IsRoot)
        {
            mIsRoot = IsRoot;
        }

        virtual bool isRoot()
        {
            return mIsRoot;
        }

		virtual Node* getParent()
		{
		    return mParent;
		}

		const virtual Node* getParent() const
		{
		    return mParent;
		}

		virtual ChildrenMapIterator getChildrenIterator()
		{
            return mChildren.begin();
		}

		virtual ChildrenMapConstIterator getChildrenIterator() const
		{
		    return mChildren.begin();
		}

		virtual Transform getTransform() const
		{
			return mTransform;
		}

		virtual void setTransform(const Transform& NewTransform)
		{
			mTransform = NewTransform;
		}

		virtual Vector3D getTranslation() const
		{
			return mTransform.Translation;
		}

		virtual Quaternion getRotation() const
		{
			return mTransform.Rotation;
		}

		virtual Vector3D getScale() const
		{
			return mTransform.Scale;
		}

		virtual void setTranslation(const Vector3D& NewTranslation)
		{
			mTransform.Translation = NewTranslation;
		}

		virtual void setRotation(const Quaternion& NewRotation)
		{
			mTransform.Rotation = NewRotation;
		}

		virtual void setScale(const Vector3D& NewScale)
		{
			mTransform.Scale = NewScale;
		}

		virtual void translate(const Vector3D& Translation, TransformSpace RelativeTo = ETS_PARENT);
		virtual void rotate(const Quaternion& Rotation, TransformSpace RelativeTo = ETS_PARENT);
		virtual void scale(const Vector3D& Scale, TransformSpace RelativeTo = ETS_PARENT);
	};
}

#if AURORA_COMPILER == AURORA_COMPILER_MSVC
#pragma warning (pop)
#endif

#endif // __AURORA_NODE_H__
