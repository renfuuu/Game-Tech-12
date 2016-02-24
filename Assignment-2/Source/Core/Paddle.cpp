#include "Paddle.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"


Paddle::Paddle(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss, rest, frict, scal) {
	// Gets the radius of the Ogre::Entity sphere
	auto var = ent->getBoundingBox();
	auto size = var.getSize();
	shape = new btBoxShape(btVector3((var.getSize().x*scale)*.65, (var.getSize().y*scale)*.3, (var.getSize().z*scale)*.65));
}

Paddle::~Paddle() {
}

void Paddle::update() {
	if (context->hit)
		MultiPlatformHelper::print("Collision!\n");
}