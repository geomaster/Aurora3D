#ifndef __SANE_ENTITY_H__
#define __SANE_ENTITY_H__
#include <Aurora.h>
using namespace Aurora;

class SaneEntity : public Aurora::Entity
{
protected:
	bool mAttachFlag, mDetachFlag, mUpdateFlag;

public:
	SaneEntity(SceneNode *Parent) : Entity(Parent)
	{
		resetFlags();
	}

	void onAttached(SceneNode *NewParent)
	{
		mAttachFlag = true;
		Entity::onAttached(NewParent);
	}

	void onDetached()
	{
		mDetachFlag = true;
		Entity::onDetached();
	}

	void update()
	{
		mUpdateFlag = true;
	}

	void resetFlags() { mAttachFlag = mDetachFlag = mUpdateFlag = false; }

	bool getAttachFlag() { return mAttachFlag; }
	bool getDetachFlag() { return mDetachFlag; }
	bool getUpdateFlag() { return mUpdateFlag; }

};

#endif