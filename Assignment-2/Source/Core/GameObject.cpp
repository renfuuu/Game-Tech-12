#include "GameObject.h"
#include "MultiPlatformHelper.h"
#include <btBulletDynamicsCommon.h>

//Add the game object to the simulator
GameObject::GameObject(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) :
	name(nme), sceneMgr(scnMgr), rootNode(node), geom(ent), scale(scal), motionState(ms), simulator(sim), tr(), inertia(), restitution(rest), friction(frict), kinematic(kin),
	needsUpdates(false), mass(mss) {
		inertia.setZero();
}

void GameObject::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	motionState->updateTransform(tr);
}

void GameObject::translate(float x, float y, float z) {
	if(kinematic) {
		rootNode->translate(x,y,z);
		updateTransform();
	}
	else {
		body->translate(btVector3(x,y,z));
	}
}

void GameObject::setPosition(float x, float y, float z) {
	if(kinematic) {
		rootNode->setPosition(x,y,z);
		updateTransform();
	}
}

Ogre::SceneNode* GameObject::getNode() {
	return rootNode;
}


void GameObject::applyForce(float x, float y, float z) {
	body->applyCentralForce(btVector3(x, y, z));
}

void GameObject::addToSimulator() {
	// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	 updateTransform();

	// rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (mass != 0.0f) 
		shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	rbInfo.m_restitution = restitution;
	rbInfo.m_friction = friction;
	body = new btRigidBody(rbInfo);
	body->setUserPointer(this);

	if (kinematic) {
		std::cout << "Kinematic\n"; 
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	context = new CollisionContext();
	cCallBack = new BulletContactCallback(*body, *context);
	simulator->addObject(this);
}