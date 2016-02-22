#pragma once

#include <btBulletDynamicsCommon.h>
// #include "GameObject.h"
/*
A struct to keep track of contact information.  Can vary depending on what needs to be tracked
*/
class GameObject;

struct CollisionContext {
	bool hit;
	const btCollisionObject* body;
	const btCollisionObject* lastBody;
	GameObject* theObject;
	float distance;
	float velNorm;
	btVector3 point;
	btVector3 normal;
	btVector3 velocity;

	CollisionContext();

	void reset();
};