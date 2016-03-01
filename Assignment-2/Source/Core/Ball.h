#pragma once

#include "GameObject.h"
#include "SoundAdapter.h"
#include "ScoreManager.h"
#include "OgreText.h"

class Ball : public GameObject {
public:
	GameObject* previousHit;
	SoundAdapter* soundAdapter;
	ScoreManager* scoreManager;
	Ogre::Timer* timer;
	Ogre::Real dt;


	Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, SoundAdapter* sa, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Ball();
	virtual void update();
	virtual void updateTransform();
	virtual void resetScore();

};

