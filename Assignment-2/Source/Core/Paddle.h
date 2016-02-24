#pragma once

#include "GameObject.h"

class Paddle : public GameObject {
public:
	Paddle(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss);
	~Paddle();
	virtual void update();
};