#include "Wall.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Wall::Wall(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss, rest, frict, scal, kin){
	auto var = ent->getBoundingBox();
	auto size = var.getSize();

	shape = new btBoxShape(btVector3((var.getSize().x*scale)/2, (var.getSize().y*scale)/2, (var.getSize().z*scale)/2));
}

Wall::~Wall(){

}

void Wall::update() {
	if (context->hit){

	}
}