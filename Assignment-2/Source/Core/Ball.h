#pragma once

#include "GameObject.h"

class Ball : public GameObject {
public:
	GameObject* previousHit;
	int ballPoints;
	Ogre::String _paddleName;

	Ball(Ogre::String nme, Ogre::String paddleName, Ogre::SceneManager* scnMgr, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Ball();
	virtual void update();
	virtual void updateTransform();
	virtual int getPoints();
	virtual void setPoints(int points);

};

