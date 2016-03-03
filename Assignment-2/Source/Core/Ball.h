#pragma once

#include "GameObject.h"

class Ball : public GameObject {
public:
	GameObject* previousHit;
	Ogre::Timer* timer;
	Ogre::Real dt;


	Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Ball();
	virtual void update();
	virtual void updateTransform();
	virtual void resetScore();
	virtual void startScore();

};

