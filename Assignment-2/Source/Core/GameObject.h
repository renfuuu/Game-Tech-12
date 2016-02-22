//How this callback works in the code:
#pragma once
#include "BulletContactCallback.h"
#include "Simulator.h"

// Simulator & GameObject inclue each other (circular reference).
class Simulator;
class CollisionContext;
class BulletContactCallback;

//Some declarations within the game object class
class GameObject {
protected:
	Ogre::String name;
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* geom;
	//OgreMotionState* motionState;

	Simulator* simulator;
	btCollisionShape* shape;
	btRigidBody* body;
	btTransform tr;
	btVector3 inertia;

	btScalar mass;
	btScalar restitution;
	btScalar friction;
	bool kinematic;
	bool needsUpdates;

	CollisionContext* context;
	BulletContactCallback* cCallBack;

	/*GameObject(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* root, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim) 
	: 	name(nme), sceneMgr(scnMgr), rootNode(root), geom(ent), motionstate(ms), 
	 	simulator(sim), shape(), body(), tr(), inertia(), mass(1), restitution(1), friction(1), kinematic(true), needsUpdates(true), 
	 	context(), cCallBack() {}
*/
/*
//Add the game object to the simulator
inline void GameObject::addToSimulator() {
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	updateTransform();
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (mass != 0.0f) shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
	rbInfo.m_restitution = restitution;
	rbInfo.m_friction = friction;
	body = new btRigidBody(rbInfo);
	body->setUserPointer(this);
	if (kinematic) {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	simID = simulator->addObject(this);
}

//Specific game object update routine
inline void Ball::update(float elapsedTime) {
	lastTime += elapsedTime;
	simulator->getWorld()->contactTest(body, *cCallBack);
	if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
		&& (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
		//Handle the hit
		lastTime = 0.0f;
	}
	context->hit = false;
}
*/
};
