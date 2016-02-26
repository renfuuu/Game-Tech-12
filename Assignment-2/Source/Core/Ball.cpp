#include "Ball.h"
#include "MultiPlatformHelper.h"


Ball::Ball(Ogre::String nme, Ogre::String paddleName, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, SoundAdapter* sa, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, scnMgr, node, ent, ms, sim, mss, rest, frict, scal, kin), previousHit(0), ballPoints(0), _paddleName(paddleName){
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);
	soundAdapter = sa;
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
		if( context->getTheObject() != previousHit && context->getTheObject()->getName() == _paddleName ) {
			soundAdapter->playSound(SoundAdapter::PADDLE_BOUNCE);
			ballPoints++;
		}
		else if ( dt > 50 ) {
			soundAdapter->playSound(SoundAdapter::WALL_BOUNCE);
		}
		dt = timer->getMilliseconds();
	}
	previousHit = context->getTheObject();
}

int Ball::getPoints(){
	return ballPoints;
}

void Ball::setPoints(int points){
	ballPoints = points;
}