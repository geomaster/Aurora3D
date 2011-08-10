#ifndef __AURORA_ENTITY_NODE_H__
#define __AURORA_ENTITY_NODE_H__
#include "AuroraPlatform.h"
#include "AuroraNode.h"

namespace Aurora
{
    class Pipeline;

    class AURORA_LIBRARY EntityNode : public Node
    {
    protected:
        Pipeline *mPipeline;
        PipelineType mPipelineType;

    public:
        EntityNode(String Name, Pipeline *Owner, Node* Parent = NULL) : Node(Name, Parent), mPipeline(Owner)
        {
        }

        Pipeline* getPipeline() const
        {
            return mPipeline;
        }

        virtual void _notifyRemoved();
        virtual void _notifyAdded();
        virtual Transform _setAbsoluteTransform(const Transform& ParentTransform, bool UpdateChildren);
    };
}

#endif // __AURORA_ENTITY_NODE_H__
