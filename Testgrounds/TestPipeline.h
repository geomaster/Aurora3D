#ifndef _TESTPIPELINE_H_
#define _TESTPIPELINE_H_
#include <AuroraPipeline.h>
#include <AuroraEntity.h>
#include <AuroraTransform.h>
#include <AuroraSceneNode.h>
#include <cstdlib>
#include <cstdio>
using namespace Aurora;

class TestEntity : public Entity
{
protected:
	String mName;
	SceneNode* mParent;
public:
	TestEntity(Pipeline* P, SceneManager* S, String N) : Entity(P,S),mName(N){;}

	void onAttached(SceneNode* p){mParent=p;}
	void onDetached(){mParent=NULL;}
	void onParentUpdated(){;}
	SceneNode* getParent(){return mParent;}
	String getName() const {return mName;}
};

class TestPipeline : public Aurora::ImmediatePipeline
{
protected:
	struct MyComp : public EntityLess
	{
		bool operator()(const Entity* one, const Entity* two)const{return one<two;}
	} mComp;

	Scene* mScene;

public:
	void queueEntity(Entity* Ent)
	{
		TestEntity* e = static_cast<TestEntity*>(Ent);
		Transform t = Ent->getParent()->getAbsoluteTransform();
		printf("got %s r(%f %f %f)\n",e->getName().c_str(), t.Rotation.getX(), t.Rotation.getY(), t.Rotation.getZ(), t.Rotation.getW());
	}

	void reserveEntitySpace(size_t){;}
	void setScene(Scene* s){mScene=s;}
	void update()
	{

	}

	EntityLess* getComparer() {return &mComp;}
};

#endif
