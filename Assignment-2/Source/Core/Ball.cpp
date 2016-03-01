#include "Ball.h"
#include "MultiPlatformHelper.h"

Ball::Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, SoundAdapter* sa, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, node, ent, ms, sim, mss, rest, frict, scal, kin), previousHit(0) {
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);
	soundAdapter = sa;
	scoreManager = new ScoreManager();
	timer = new Ogre::Timer();
	dt = timer->getMilliseconds();
}

Ball::~Ball() {
}

void Ball::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	motionState->updateTransform(tr);
}

void Ball::update() {
	if (context->hit) {
		dt = timer->getMilliseconds() - dt;
		if( context->getTheObject() != previousHit && context->getTheObject()->getType() == GameObject::PADDLE_OBJECT ) {
			soundAdapter->playSound(SoundAdapter::PADDLE_BOUNCE);
			scoreManager->scorePoints(1);
		}
		else if ( dt > 5 ) {
			soundAdapter->playSound(SoundAdapter::WALL_BOUNCE);
			if ( context->getTheObject()->getType() == GameObject::FLOOR_OBJECT ) {
				if ( !(scoreManager->floorHit()) ) {
					this->resetScore();
					return;
				}
			}
			else
				scoreManager->nonFloorHit();
		}
		
		if( context->getTheObject()->getType() == GameObject::BACK_WALL_OBJECT && previousHit->getType() == GameObject::PADDLE_OBJECT ) {
			soundAdapter->playSound(SoundAdapter::HEADSHOT);
			scoreManager->scorePoints(1);
		}

		dt = timer->getMilliseconds();
	}
	previousHit = context->getTheObject();
}

void Ball::resetScore() {
	this->GameObject::reset();
	scoreManager->resetScore();
}