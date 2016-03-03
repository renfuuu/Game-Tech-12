#include "Ball.h"
#include "MultiPlatformHelper.h"

Ball::Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin), previousHit(0) {
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);

	timer = new Ogre::Timer();
	dt = timer->getMilliseconds();

	Ogre::SceneNode* particleNode = rootNode->createChildSceneNode("Particle");
	particleNode->attachObject(particle);
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

	startScore();

	if (context->hit) {
		dt = timer->getMilliseconds() - dt;
		if( context->getTheObject() != previousHit && context->getTheObject()->getType() == GameObject::PADDLE_OBJECT ) {
			soundScoreManager->playSound(SoundScoreManager::PADDLE_BOUNCE);
			soundScoreManager->scorePoints(1);
		}
		else if ( dt > 5 ) {
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

		dt = timer->getMilliseconds();
	}
	previousHit = context->getTheObject();
}