#include "Ball.h"
#include "MultiPlatformHelper.h"


Ball::Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, SoundAdapter* sa, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, node, ent, ms, sim, mss, rest, frict, scal, kin), previousHit(0), ballPoints(0) {
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
		if( context->getTheObject() != previousHit && context->getTheObject()->getType() == GameObject::PADDLE_OBJECT ) {
			soundAdapter->playSound(SoundAdapter::PADDLE_BOUNCE);
			ballPoints++;
			floorHitCount = 0;
		}
		else if ( dt > 50 ) {
			soundAdapter->playSound(SoundAdapter::WALL_BOUNCE);
		}
		
		if ( context->getTheObject()->getType() == GameObject::FLOOR_OBJECT && dt > 50 ) {
			floorHitCount++;
			if ( floorHitCount >= 2 ) {
				std::cout << "Game Over\n";
				gameOver();
			}
		}
		else if ( dt > 50 ) {
			floorHitCount = 0;
		}
		
		if( context->getTheObject()->getType() == GameObject::BACK_WALL_OBJECT && previousHit->getType() == GameObject::PADDLE_OBJECT )
			std::cout << "HeadShot!\n";
		
		dt = timer->getMilliseconds();
	}
	previousHit = context->getTheObject();
}

void Ball::gameOver() {
	std::cout << "Game Over\n";
	//Post Game Over Text
	MultiPlatformHelper::sleep(1000);
	floorHitCount = 0;
	//Reset
	this->reset();
	this->setPoints(0);
}

int Ball::getPoints(){
	return ballPoints;
}

void Ball::setPoints(int points){
	ballPoints = points;
}