#pragma once

#include "GameObject.h"
#include "SoundAdapter.h"

class Ball : public GameObject {
public:
	GameObject* previousHit;
	SoundAdapter* soundAdapter;
	int ballPoints;
	int floorHitCount;
	Ogre::Timer* timer;
	Ogre::Real dt;


	Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, SoundAdapter* sa, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Ball();
	virtual void update();
	virtual void updateTransform();
	virtual void gameOver();
	virtual int getPoints();
	virtual void setPoints(int points);

};

