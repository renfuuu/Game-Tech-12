#pragma once

#include "GameObject.h"

class Ball : public GameObject {
public:
	Ball(Ogre::String nme, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss);
	~Ball();
	virtual void update();
};

