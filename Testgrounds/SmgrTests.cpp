#include "TestEntity.h"
#include <AuroraScene.h>
#include <AuroraSceneManager.h>
#include <AuroraSceneNode.h>
#include <AuroraException.h>

using namespace Aurora;

int main()
{
	SceneManager *smgr = AURORA_NEW SceneManager();
	Scene* scene = AURORA_NEW Scene();
	smgr->setScene(scene);
	SceneNode* m = scene->getRootSceneNode()->createChildSceneNode("IHasANode");
	SceneNode *m2 = m->createChildSceneNode("ChildNode1");
	m2->setTranslation(Vector3D(0.f, 0.f, 1.f));
	m->setRotation(Quaternion(Vector3D(0.f, 1.f, 0.f), Degree(90.f)));
	m->setTranslation(Vector3D(0.f, 1.f, 0.f));

	TestEntity *yvonne = AURORA_NEW TestEntity("Yvonne");
	TestEntity *michael = AURORA_NEW TestEntity("Michael");

	m2->attachEntity(yvonne);
	m->attachEntity(michael);

	puts("***********");
	smgr->update();
	puts("***********");

	m2->detachEntity(yvonne);
	m->attachEntity(yvonne);

	// NASTY!
	scene->getRootSceneNode()->translate(Vector3D(0.f, 1.f, 0.f));

	puts("***********");
	smgr->update();
	puts("***********");

	m->detachEntity(yvonne);
	m->detachEntity(michael);
	m2->attachEntity(michael);

	puts("***********");
	smgr->update();
	puts("***********");

	{char c;scanf("%c",&c);}
	return 0;
}
