#ifndef __TEST_ENTITY_H__
#define __TEST_ENTITY_H__
#include <AuroraEntity.h>
#include <AuroraSceneNode.h>
#include <cstdlib>

using namespace Aurora;

class TestEntity : public Entity
{
protected:
	String mName;

public:
	TestEntity(String Name, SceneNode* Parent = NULL) : Entity(Parent), mName(Name)
	{
	}

	void onAttached(SceneNode* NewParent)
	{
		printf("%s is attached to %s\n", mName.c_str(), NewParent->getName().c_str());
		Entity::onAttached(NewParent);
	}

	void onDetached()
	{
		printf("%s is lonely\n", mName.c_str());
		Entity::onDetached();
	}

	void update()
	{
		Transform t = mParent->getAbsoluteTransform();
		printf("my name is %s and i love noodles (%f %f %f)\n", mName.c_str(), t.Translation.getX(), t.Translation.getY(), t.Translation.getZ());
	}
};

#endif