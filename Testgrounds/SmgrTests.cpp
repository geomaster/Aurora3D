#include "TestPipeline.h"
#include <AuroraScene.h>
#include <AuroraSceneManager.h>
#include <AuroraSceneNode.h>
using namespace Aurora;

int main()
{
	SceneManager *smgr = AURORA_NEW SceneManager();
	Scene* scene = AURORA_NEW Scene();
	TestPipeline *pipe = AURORA_NEW TestPipeline();
	pipe->setScene(scene);
	SceneNode* m = scene->getRootSceneNode()->createChildSceneNode("MyNode");
	TestEntity *e = AURORA_NEW TestEntity(pipe, smgr, "Bob");

	smgr->addPipeline(pipe);
	smgr->setScene(scene);
	smgr->_registerEntity(e);

	smgr->onFrameStart();
	smgr->update();
	smgr->onFrameEnd();

	{char c;scanf("%c",&c);}
	return 0;
}
