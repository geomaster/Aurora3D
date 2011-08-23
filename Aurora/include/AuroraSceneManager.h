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
#ifndef __AURORA_SCENE_MANAGER_H__
#define __AURORA_SCENE_MANAGER_H__
#include "AuroraPrereqs.h"
#include "AuroraScene.h"
#include "AuroraTransform.h"
#include "STL/Vector.h"
#include "STL/Set.h"
#include "STL/HashMap.h"

namespace Aurora
{
	class AURORA_LIBRARY SceneManager : virtual public Alloc
	{
	protected:
		Scene *mScene;
		void updateSceneGraph(SceneNode *Start, const Transform& ParentTransform = Transform(), bool Force = false);
		
	public:
		SceneManager();

		virtual void setScene(Scene *NewScene);
		inline virtual Scene* getScene() const
		{
			return mScene;
		}

		virtual void update();

		virtual ~SceneManager() { ; }
	};
}

#endif // __AURORA_SCENE_MANAGER__
