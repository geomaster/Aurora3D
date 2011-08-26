#include <cstdio>
#include <iostream>
#include "SaneEntity.h"
#include <Aurora.h>
using namespace Aurora;
using namespace std;

void sceneMemoryManagementTest()
{
	// Do some fuzz testing on Scene's memory management
	Scene * scene = AURORA_NEW Scene();
	SceneNode *s1 = scene->createSceneNode("1"), *s2 = scene->createSceneNode("2"), *s3 = scene->createSceneNode("3");
	SceneNode *sc1 = s1->createChildSceneNode("1>1"), *sc2 = s2->createChildSceneNode("2>1");

	// These are just checks if we got a nice memory location; if not,
	// these asserts will crash terribly (segfaults) or just fail
	assert(s1->getName() == "1" && s2->getName() == "2" && s3->getName() == "3");
	assert(sc1->getName() == "1>1" && sc2->getName() == "2>1");

	scene->destroySceneNode(s2);
	assert(!sc2->getParent());

	// Tribute to thrash: Metallica

	SceneNode *recycl1 = scene->createSceneNode("For Whom The Bell Tolls");
	assert(recycl1->getName() == "For Whom The Bell Tolls");

	scene->destroySceneNode(s1);
	scene->destroySceneNode(sc1);
	scene->destroySceneNode(sc2);
	
	// ...and Kreator.
	SceneNode *recycl2 = scene->createSceneNode("Extreme Aggression");
	assert(recycl2->getName() == "Extreme Aggression");

	scene->destroySceneNode(recycl1);
	scene->destroySceneNode(recycl2);

	AURORA_DELETE scene;

	cout << "Below is a memory tracker report. Lines that belong to AuroraScene.cpp\n"
			"are most likely memory leaks. If there are no lines in AuroraScene.cpp\n"
			"or no lines at all - good job! You're being a good citizen and clean\n"
			"up behind yourself.\n\n";
	MemoryTracker::getSingleton().report(cout);
}

void sceneNodeUpdateTest()
{
	// Do some test to see if scene nodes update themselves
	// nicely.
	Scene* scene = AURORA_NEW Scene();

	// sn_[level in hierarchy]_[id]
	SceneNode *sn_1_0 = scene->createSceneNode("Realit Atckontrolle"),
		*sn_1_1 = scene->createSceneNode("Reborn"),
		*sn_1_2 = scene->createSceneNode("The Four Horsemen");

	SceneNode *sn_2_0 = sn_1_0->createChildSceneNode("Creeping Death"),
		*sn_3_0 = sn_2_0->createChildSceneNode("Trapped Under Ice"),
		*sn_2_1 = sn_1_1->createChildSceneNode("One Evil Comes (A Million Follow)");

	/****************
	^
	|
	z x->
	
	        2_0
		   root

	****************/
	sn_2_0->setPosition(Vector3D(0.f, 0.f, 1.f));


	/***************
	^
	x z>
	      root 2_0

    ***************/
	sn_1_0->rotate(Quaternion(Vector3D(0.f, 1.f, 0.f), Degree(90)));

	Vector3D _p1 = sn_3_0->getAbsoluteTransform().Position;
	Vector3D _p2 = sn_2_0->getAbsoluteTransform().Position;

	assert(_p1.equals(_p2) && _p1.equals(Vector3D(1.f, 0.f, 0.f)));

	sn_1_0->rotate(Quaternion(Vector3D(0.f, 1.f, 0.f), Degree(-90)));

	Vector3D _p3 = sn_2_0->getAbsoluteTransform().Position;

	assert(_p3.equals(Vector3D(0.f, 0.f, 1.f)));

	sn_1_1->setPosition(Vector3D(0.f, 1.f, 0.f));
	sn_2_1->setPosition(Vector3D(1.f, 0.f, 0.f));

	Vector3D _p4 = sn_2_1->getAbsoluteTransform().Position;

	assert(_p4.equals(Vector3D(1.f, 1.f, 0.f)));

	cout << "If none of the assertions failed (and you probably wouldn't\n"
			"be reading this text if they did) then this looks fine.\n"
			"But, Bob, I'd never be sure. Be sure to keep an eye on\n"
			"these implementations, especially on SceneNode. It's\n"
			"grinning at the moment and holding a spatula... Bob,\n"
			"something's not quite right.";

	AURORA_DELETE scene;
}

void entityNotificationTest()
{
	Scene *scene = AURORA_NEW Scene();
	SceneManager *smgr = AURORA_NEW SceneManager();
	smgr->setScene(scene);

	SaneEntity *a = AURORA_NEW SaneEntity(NULL);
	a->resetFlags();

	SceneNode *belowRoot = scene->createSceneNode("Violent Revolution");
	belowRoot->attachEntity(a);

	assert(a->getAttachFlag());
	a->resetFlags();

	belowRoot->detachEntity(a);
	
	assert(a->getDetachFlag());
	a->resetFlags();

	SceneNode *others = belowRoot->createChildSceneNode("Whiplash");
	SaneEntity *b = AURORA_NEW SaneEntity(NULL);

	others->attachEntity(b);
	b->resetFlags();

	smgr->update();

	assert(b->getUpdateFlag());

	std::cout << "If all assertions passed, you can sleep peacefully at night\n"
				"knowing that Entities will be informed should the need arise.\n"
				"(not really)";


	AURORA_DELETE scene, AURORA_DELETE smgr, AURORA_DELETE a, AURORA_DELETE b;

}

int main()
{
	sceneMemoryManagementTest();
	cout << "\n\n******************\n\n";

	sceneNodeUpdateTest();
	cout << "\n\n******************\n\n";

	entityNotificationTest();
	cout << "\n\n***** END OF SCENEGRAPH-RELATED SANITY CHECKS **** \n\n";

	MemoryTracker::getSingleton().report(cout);

	cin.get();
	return 0;
}