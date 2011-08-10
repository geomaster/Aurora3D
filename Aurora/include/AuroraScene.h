#ifndef __AURORA_SCENE_H__
#define __AURORA_SCENE_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include <STL/Vector.h>

namespace Aurora
{
	class Scene : virtual public Alloc
	{
	protected:
		SceneNode* mRootNode;
		SceneManager* mSceneManager;
		STL::Vector<SceneNode*>::type mCreatedNodes;

	public:
		Scene();

		virtual void createSceneNode();
		virtual void destroySceneNode(SceneNode *ToDestroy);

		virtual void setSceneManager(SceneManager* NewMgr)
		{
			mSceneManager = NewMgr;
		}

		virtual SceneManager* getSceneManager()
		{
			return mSceneManager;
		}

		virtual SceneNode* getRootSceneNode()
		{
			return mRootNode;
		}
	};
};

#endif // __AURORA_SCENE_H__
