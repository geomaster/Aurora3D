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
#include "AuroraNode.h"
#include <STL/HashMap.h>

namespace Aurora
{
	class SceneNode : public Node
	{
	protected:
		Scene* mScene;
		STL::HashMap<String, Entity*>::type mEntities;

	public:
		SceneNode(String Name, Scene* Owner, Node* Parent = NULL) : Node(Name, Parent), mScene(Owner) { ; }

        virtual Node* createChild(String Name, const Transform& ChildTransform = Transform())
        {
        	return createChildSceneNode(Name, ChildTransform);
        }

        virtual Node* createChild(String Name, const Vector3D& Translation = Vector3D::Zero,
                         const Quaternion& Rotation = Quaternion::Identity,
                         const Vector3D& Scale = Vector3D(Real( 1.0 )))
		{
			return createChildSceneNode(Name, Translation, Rotation, Scale);
		}

		virtual SceneNode* createChildSceneNode(String Name, const Transform& ChildTransform = Transform());
		virtual SceneNode* createChildSceneNode(String Name, const Vector3D& Translation = Vector3D::Zero,
                         const Quaternion& Rotation = Quaternion::Identity,
                         const Vector3D& Scale = Vector3D(Real( 1.0 )));

		virtual void attachEntity(Entity* NewEntity);
		virtual void detachEntity(Entity* OldEntity);
		virtual void detachEntity(String Name);

		virtual Entity* getEntityByName(String Name);
		virtual bool hasEntity(Entity* Ent);
	};
}

#endif // __AURORA_SCENE_NODE_H__
