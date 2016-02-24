#include "Paddle.h"
#include "MultiPlatformHelper.h"


Paddle::Paddle(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss) {
	// Gets the radius of the Ogre::Entity sphere
	auto var = ent->getBoundingBox();
	shape = new btBoxShape(btVector3(var.getSize().x, var.getSize().y, var.getSize().z));
}

Paddle::~Paddle() {
}

void Paddle::update() {
	if (context->hit)
		MultiPlatformHelper::print("Collision!\n");
}