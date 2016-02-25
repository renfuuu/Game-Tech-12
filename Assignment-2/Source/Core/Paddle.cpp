#include "Paddle.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"


Paddle::Paddle(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	kinematic = true;
	auto var = ent->getBoundingBox();
	auto size = var.getSize();

	// We scale the y lower than .5 because the handle increases the bounding box's size
	shape = new btBoxShape(btVector3((var.getSize().x*scale)/2, (var.getSize().y*scale)*2, (var.getSize().z*scale)*.75));
}

Paddle::~Paddle() {
}

void Paddle::update() {
	if (context->hit) {
		
	}
}