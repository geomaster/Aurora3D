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
	SceneNode *m2 = m->createChildSceneNode("MyNewNode");
	m2->setTranslation(Vector3D(0.f, 0.f, 1.f));
	m2->setRotation(Quaternion(Vector3D(0.f, 1.f, 0.f), Degree(-90.f)));

	TestEntity *e = AURORA_NEW TestEntity(pipe, smgr, "Bob");
	m->setRotation(Quaternion(Vector3D(0.f, 1.f, 0.f), Degree(90.f)));

	m2->attachEntity(e);
	smgr->addPipeline(pipe);
	smgr->setScene(scene);

	smgr->update();

	{char c;scanf("%c",&c);}
	return 0;
}
