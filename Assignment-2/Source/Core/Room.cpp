#include "Room.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Room::Room(){

}

Room::~Room(){

}

// Ball* Application::createBall(Ogre::String nme, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
// 	createRootEntity(nme, meshName, x, y, z);
// 	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
// 	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
// 	const btTransform pos;
// 	OgreMotionState* ms = new OgreMotionState(pos, sn);
// 	sn->setScale(scale,scale,scale);

// 	Wall* obj = new Wall(nme, mSceneManager, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
// 	obj->addToSimulator();

// 	return obj;
// }