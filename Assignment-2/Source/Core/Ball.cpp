#include "Ball.h"
#include "MultiPlatformHelper.h"


Ball::Ball(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss) {
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape(ent->getBoundingBox().getHalfSize().x - 2);
}

Ball::~Ball() {
}

void Ball::update() {
	if (context->hit)
		MultiPlatformHelper::print("Collision!\n");
}