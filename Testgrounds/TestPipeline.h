#ifndef _TESTPIPELINE_H_
#define _TESTPIPELINE_H_
#include <AuroraPipeline.h>
#include <AuroraEntity.h>
#include <cstdlib>
#include <cstdio>
using namespace Aurora;

class TestEntity : public Entity
{
protected:
	String mName;
public:
	TestEntity(Pipeline* P, SceneManager* S, String N) : Entity(P,S),mName(N){;}

	void onAttached(SceneNode*){;}
	void onDetached(){;}
	void onParentUpdated(){;}
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
	bool onFrameStart()
	{
		system("clear");
		return true;
	}

	void update(){;}
	bool onFrameEnd()
	{
		return true;

	}

	void queueEntity(Entity* Ent)
	{
		TestEntity* e = static_cast<TestEntity*>(Ent);
		printf("got %s\n",e->getName().c_str());
	}

	void reserveEntitySpace(size_t){;}
	void setScene(Scene* s){mScene=s;}

	EntityLess* getComparer() {return &mComp;}
};

#endif
