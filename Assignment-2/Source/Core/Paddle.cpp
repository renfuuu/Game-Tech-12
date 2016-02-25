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

void Paddle::movePaddle(OISManager* _oisManager, int height, int width) {
	int mouseX = _oisManager->getMouseXAxis();
	int mouseY = Ogre::Math::Clamp(_oisManager->getMouseYAxis(), -100, 220);
	int paddleZ = (height/4)-(Ogre::Math::Sqr(mouseX)/width + Ogre::Math::Sqr((width/height)*1.5*mouseY)/height);

	Ogre::SceneNode* mNode = rootNode;

	Ogre::Vector3 surfacePoint(mouseX, paddleZ, mouseY);
	Ogre::Quaternion orient = mNode->getOrientation();
	Ogre::Vector3 normal = surfacePoint.normalisedCopy();
	Ogre::Vector3 normalCopy = -(surfacePoint + Ogre::Vector3(0,0,0)).normalisedCopy();
	Ogre::Vector3 ortho1 = (Ogre::Vector3(0, 1, 0).crossProduct(normalCopy)).normalisedCopy();
	Ogre::Vector3 ortho2 = (normalCopy.crossProduct(ortho1)).normalisedCopy();

	Ogre::Quaternion newOrientation(ortho1, ortho2, normalCopy);
	mNode->setOrientation(newOrientation);

	this->setPosition(surfacePoint + normal*50);

	if (mouseX < 0) {
		Ogre::Vector3 u = newOrientation.yAxis();
		Ogre::Vector3 v = Ogre::Vector3(0, 0, 1);
		Ogre::Real cosine = u.dotProduct(v);

		Ogre::Real sin = u.crossProduct(v).length();
		mNode->roll(Ogre::Math::ATan2(sin, cosine));
	}
	else {
		Ogre::Vector3 u = -newOrientation.yAxis();
		Ogre::Vector3 v = Ogre::Vector3(0, 0, 1);
		Ogre::Real cosine = u.dotProduct(v);

		Ogre::Real sin = u.crossProduct(v).length();
		mNode->roll(Ogre::Math::ATan2(sin, cosine));
	}
}