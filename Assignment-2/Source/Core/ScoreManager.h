 #pragma once

#include "OgreText.h"
#include "GameObject.h"
#include "TextOverlay.h"
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
 	int highScore;
 	int floorHitCount;
 	//OgreText* score;
 	TextOverlay* scoreOverlay;
 	TextOverlay* highScoreOverlay;
 	TextOverlay* gameOverOverlay;
	Ogre::String scoreText;
	Ogre::String scoreLabel;
	Ogre::String highScoreText;
	Ogre::String highScoreLabel;
	Ogre::String gameOverText;
	Ogre::String gameOverLabel;

 };