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
#ifndef __AURORA_PIPELINE_H__
#define __AURORA_PIPELINE_H__
#include "AuroraPrereqs.h"
#include "AuroraIterator.h"

namespace Aurora
{
	enum PipelineType
	{
		EPT_Immediate,
		EPT_Listener
	};

	class Pipeline : virtual public Alloc
	{
	public:
		virtual PipelineType getType() = 0;
		virtual void update() = 0;
		virtual void setScene(Scene*) = 0;
	};

	class ImmediatePipeline : public Pipeline
	{
	public:
		struct EntityLess
		{
			virtual bool operator()(const Entity*, const Entity*) const = 0;
		};

		PipelineType getType()
		{
			return EPT_Immediate;
		}

		//virtual void setEntityList(ContainerForwardIterator<Entity*>*) = 0;
		virtual void queueEntity(Entity*) = 0;
		virtual void reserveEntitySpace(size_t Count) = 0;

		virtual EntityLess* getComparer() = 0;
	};

	class ListenerPipeline : public Pipeline
	{
	public:
		PipelineType getType()
		{
			return EPT_Listener;
		}
	};
}
#endif // __AURORA_PIPELINE_H__
