#ifndef __AURORA_PIPELINE_H__
#define __AURORA_PIPELINE_H__
#include "AuroraPrereqs.h"
#include "AuroraNode.h"
#include "AuroraIterator.h"

namespace Aurora
{
	enum PipelineType
	{
		EPT_Immediate,
		EPT_Listener
	};

	class Pipeline
	{
	public:
		virtual PipelineType getType() = 0;

		virtual bool onFrameStart() = 0;
		virtual void update() = 0;
		virtual bool onFrameEnd() = 0;

		virtual void setScene(Scene*) = 0;
	};

	class ImmediatePipeline : public Pipeline
	{
	public:
		PipelineType getType()
		{
			return EPT_Immediate;
		}

		virtual void setEntityList(ContainerForwardIterator<Entity*>*) = 0;
		virtual void queueEntity(Entity*) = 0;
	};

	class ListenerPipeline : public Pipeline
	{
	public:
		PipelineType getType()
		{
			return EPT_Listener;
		}

		virtual void onNodeUpdated(SceneNode*) = 0;
		virtual void onNodeCreated(SceneNode*) = 0;
		virtual void onNodeQueuedDesroying(SceneNode*) = 0;
		virtual void onNodeDetached(SceneNode*, Node*) = 0;
		virtual void onNodeAttached(SceneNode*, Node*) = 0;
	};
}
#endif // __AURORA_PIPELINE_H__
