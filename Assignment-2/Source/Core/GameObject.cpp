#include "GameObject.h"
#include <btBulletDynamicsCommon.h>

//Add the game object to the simulator
GameObject::GameObject(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim) :
	name(nme), sceneMgr(scnMgr), rootNode(node), geom(ent), motionState(ms), simulator(sim), shape(), tr(), inertia(), mass(1), restitution(1), friction(), kinematic(true),
	needsUpdates(false) {
		// body = new btRigidBody(mass, motionState, shape, inertia);
	}

void GameObject::addToSimulator() {
	// //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	// updateTransform();
	//rigidbody is dynamic if and only if mass is non zero, otherwise static

	// CalculateLocalIntertia is purely virtual
	// if (mass != 0.0f) shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	rbInfo.m_restitution = restitution;
	rbInfo.m_friction = friction;
	body = new btRigidBody(rbInfo);
	body->setUserPointer(this);
	if (kinematic) {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	simulator->addObject(this);
}