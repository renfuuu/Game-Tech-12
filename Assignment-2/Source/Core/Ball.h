#pragma once

#include "GameObject.h"
#include "SoundAdapter.h"

class Ball : public GameObject {
public:
	GameObject* previousHit;
	SoundAdapter* soundAdapter;
	int ballPoints;
	Ogre::Timer* timer;
	Ogre::Real dt;
	Ogre::String _paddleName;

	Ball(Ogre::String nme, Ogre::String paddleName, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, SoundAdapter* sa, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Ball();
	virtual void update();
	virtual void updateTransform();
	virtual int getPoints();
	virtual void setPoints(int points);

};

