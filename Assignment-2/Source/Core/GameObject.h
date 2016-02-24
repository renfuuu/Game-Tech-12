//How this callback works in the code:
#pragma once

#include "BulletContactCallback.h"
#include "Simulator.h"
#include "CollisionContext.h"
#include "OgreMotionState.h"

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h>

// Simulator & GameObject inclue each other (circular reference).

//Some declarations within the game object class
class GameObject {
protected:
	Ogre::String name;
	Ogre::SceneManager* sceneMgr;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* geom;
	Ogre::Real scale;
	OgreMotionState* motionState;

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

public:
	BulletContactCallback* cCallBack;

	GameObject(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	inline btRigidBody* getBody() { return body; }
	void addToSimulator();
	void updateTransform();
	void translate(float x, float y, float z);
	void applyForce(float x, float y, float z);
	void setPosition(float x, float y, float z);
	Ogre::SceneNode* getNode();
	virtual void update() = 0;
};
