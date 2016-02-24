#include "Paddle.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"


Paddle::Paddle(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss) {
	// Gets the radius of the Ogre::Entity sphere
	auto var = ent->getBoundingBox();
	auto size = var.getSize();
	shape = new btBoxShape(btVector3(var.getSize().x*100 - 1, var.getSize().y*100 - 5, var.getSize().z*100 - 1));
}

Paddle::~Paddle() {
}

void Paddle::update() {
	if (context->hit)
		MultiPlatformHelper::print("Collision!\n");
}