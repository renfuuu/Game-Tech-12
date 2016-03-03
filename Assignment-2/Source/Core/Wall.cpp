#include "Wall.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Wall::Wall(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin), previousHit(0) {
	auto var = ent->getBoundingBox();
	auto size = var.getSize();

	shape = new btBoxShape(btVector3((var.getSize().x*vscale.x)/2, (var.getSize().y*vscale.y)/2, (var.getSize().z*vscale.z)/2));
}

Wall::~Wall(){
	if (context->hit) {
		soundScoreManager->playSound(SoundScoreManager::WALL_BOUNCE);
		soundScoreManager->setDT(soundScoreManager->getDT());
		if( context->getTheObject() != previousHit && context->getTheObject()->getType() == GameObject::PADDLE_OBJECT ) {
			soundScoreManager->playSound(SoundScoreManager::PADDLE_BOUNCE);
			soundScoreManager->scorePoints(1);
		}
		else if ( soundScoreManager->getDT() > 10 ) {
			if ( context->getTheObject()->getType() == GameObject::FLOOR_OBJECT ) {
				if ( !(soundScoreManager->floorHit()) ) {
					this->resetScore();
					return;
				}
			}
			else
				soundScoreManager->nonFloorHit();
		}

		if( context->getTheObject()->getType() == GameObject::BACK_WALL_OBJECT && previousHit->getType() == GameObject::PADDLE_OBJECT ) {
			soundScoreManager->playSound(SoundScoreManager::HEADSHOT);
			soundScoreManager->scorePoints(1);
		}

		soundScoreManager->setDT();
	}
	previousHit = context->getTheObject();
}

void Wall::update() {	

}