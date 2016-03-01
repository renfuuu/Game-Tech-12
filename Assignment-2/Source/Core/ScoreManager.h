 #pragma once

#include "OgreText.h"
#include "GameObject.h"
#include "MultiPlatformHelper.h"

 class ScoreManager {
 public:
 	ScoreManager(void);
 	~ScoreManager(void);

 	void postScore(void);
 	void postHighScore(void);
 	void scorePoints(int);
 	void nonFloorHit(void);
 	bool floorHit(void);
 	void resetScore(void);
 	void gameOver(void);

 protected:
 	int gameScore;
 	int floorHitCount;
 	OgreText* score;
	Ogre::String scoreText;

 };