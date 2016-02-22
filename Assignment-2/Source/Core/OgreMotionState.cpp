#include "OgreMotionState.h"

OgreMotionState::OgreMotionState(const btTransform &initialpos, Ogre::SceneNode* node) {
	mVisibleobj = node;
	mPos1 = initialpos;
}

	//Provides flexibility in terms of object visibility
void OgreMotionStatesetNode(Ogre::SceneNode* node) {
	mVisibleobj = node;
} 

virtual void OgreMotionStategetWorldTransform(btTransform &worldTrans) const {
	worldTrans = mPos1;
}

virtual void OgreMotionStatesetWorldTransform(const btTransform &worldTrans) {
	if (mVisibleobj == nullptr)
		return; // silently return before we set a node

		btQuaternion rot = worldTrans.getRotation();
		mVisibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
		btVector3 pos = worldTrans.getOrigin();
		mVisibleobj->setPosition(pos.x(), pos.y(), pos.z());
}