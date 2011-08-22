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
#include "AuroraPipeline.h"
#include <STL/Vector.h>
#include <STL/Set.h>
#include <STL/HashMap.h>

namespace Aurora
{
	class AURORA_LIBRARY SceneManager : virtual public Alloc
	{
	public:
		typedef STL::Vector<Pipeline*>::type PipelineList;
		typedef PipelineList::iterator PipelineListIterator;
		typedef PipelineList::const_iterator PipelineListConstIterator;


	protected:
		Scene *mScene;
		PipelineList mPipelines;

		class EntitySpec
		{
		protected:
			ImmediatePipeline::EntityLess* mComp;

		public:
			ImmediatePipeline* Owner;
			Entity* Ent;

			EntitySpec(Entity* E)
			{
				Ent = E, Owner = NULL, mComp = NULL;
			}

			EntitySpec(Entity* E, ImmediatePipeline* O)
			{
				Ent = E, Owner = O;
				mComp = O->getComparer();
			}

			inline bool operator<(const EntitySpec& other) const
			{
				return (mComp? mComp->operator()(Ent, other.Ent) : false);
			}

			inline bool operator==(const EntitySpec& other) const
			{
				return Ent == other.Ent;
			}
		};

		typedef STL::Set<EntitySpec>::type EntityList;
		typedef EntityList::iterator EntityListIterator;
		typedef EntityList::const_iterator EntityListConstIterator;

		typedef STL::HashMap<Pipeline*, EntityList>::type PipelineEntityList;
		typedef PipelineEntityList::iterator PipelineEntityListIterator;
		typedef PipelineEntityList::const_iterator PipelineEntityListConstIterator;

		PipelineEntityList mEntities;

		void registerEntities(SceneNode* From);
	public:
		SceneManager();

		virtual void setScene(Scene *NewScene);

		inline virtual Scene* getScene() const
		{
			return mScene;
		}

		virtual void addPipeline(Pipeline *P);
		virtual void removePipeline(Pipeline *ToRemove);
		virtual PipelineList& getPipelineList()
		{
			return mPipelines;
		}
		virtual const PipelineList& getPipelineList() const
		{
			return mPipelines;
		}

		// Should be called by immediate pipelines to (un)register their Entities, not by
		// listener ones
		virtual void _registerEntity(Entity* Ent);
		virtual void _unregisterEntity(Entity *Ent);

		virtual void update();

		virtual ~SceneManager() { ; }
	};
}

#endif // __AURORA_SCENE_MANAGER__
