#include "Ball.h"
#include "MultiPlatformHelper.h"

Ball::Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);

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

	if (context->hit) {

		startScore();

		// Check for paddle collision but not twice in a row
		if( context->getTheObject()->getType() == GameObject::PADDLE_OBJECT && context->getTheObject() != previousHit ) {
			soundScoreManager->playSound(SoundScoreManager::PADDLE_BOUNCE);
			soundScoreManager->nonFloorHit();
		}		

		//Score only when you hit behind your opponent.
		if( context->getTheObject()->getType() == GameObject::BACK_WALL_OBJECT ) {
			soundScoreManager->scorePoints(1);
		}
		previousHit = context->getTheObject();
	}
}

std::string Ball::getCoordinates() {
	btVector3 velocity = getBody()->getLinearVelocity();
	std::string px = std::to_string(rootNode->getPosition().x);
	std::string py = std::to_string(rootNode->getPosition().y);
	std::string pz = std::to_string(rootNode->getPosition().z);
	std::string vx = std::to_string(velocity.x());
	std::string vy = std::to_string(velocity.y());
	std::string vz = std::to_string(velocity.z());

	std::string str = "BPX " + px + "\nBPY " + py + "\nBPZ " + pz + "\nBVX " + vx + "\nBVY " + vy + "\n BVZ " + vz;
	return str;
}

std::string Ball::getPoints() {
	std::string str = "SCR " + std::to_string(soundScoreManager->getGameScore());
	return str;
}