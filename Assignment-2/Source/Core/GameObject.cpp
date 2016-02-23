#include "GameObject.h"
#include "MultiPlatformHelper.h"
#include <btBulletDynamicsCommon.h>

//Add the game object to the simulator
GameObject::GameObject(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss) :
	name(nme), sceneMgr(scnMgr), rootNode(node), geom(ent), motionState(ms), simulator(sim), tr(), inertia(), restitution(1), friction(), kinematic(false),
	needsUpdates(false), mass(mss) {
		// mass = 1.0f;
		inertia.setZero();

		// TODO remove this and have children decide their shapes
		shape = new btSphereShape(100);
}

void GameObject::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	motionState->updateTransform(tr);
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
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}

	context = new CollisionContext();
	cCallBack = new BulletContactCallback(*body, *context);
	simulator->addObject(this);
}

// TODO Make this virtual and only have children create this method
void GameObject::update() {
	if(context->hit)
		MultiPlatformHelper::print("Collision!\n");
}