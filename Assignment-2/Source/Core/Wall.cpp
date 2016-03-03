#include "Wall.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Wall::Wall(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin), previousHit(0) {
	auto var = ent->getBoundingBox();
	auto size = var.getSize();

	timer = new Ogre::Timer();
	dt = timer->getMilliseconds();

	shape = new btBoxShape(btVector3((var.getSize().x*vscale.x)/2, (var.getSize().y*vscale.y)/2, (var.getSize().z*vscale.z)/2));
}

Wall::~Wall(){

}

void Wall::update() {	
	if (context->hit) {
		dt = timer->getMilliseconds() - dt;
		
		if ( dt > 5 )
			soundScoreManager->playSound(SoundScoreManager::WALL_BOUNCE);

		dt = timer->getMilliseconds();
	}
	
	previousHit = context->getTheObject();
}