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
		Transform t = e->getParent()->getAbsoluteTransform();
		Vector3D a = t.toMatrix() * Vector3D(0.f);

		printf("got %s (%f %f %f)\n",e->getName().c_str(), a.getX(), a.getY(), a.getZ());
	}

	void reserveEntitySpace(size_t){;}
	void setScene(Scene* s){mScene=s;}
	void update()
	{

	}

	EntityLess* getComparer() {return &mComp;}
};

#endif
