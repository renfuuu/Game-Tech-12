 #pragma once

#include "GameObject.h"
#include "TextOverlay.h"
#include "MultiPlatformHelper.h"
#include <fstream>
#include <iostream>

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
 	void writeScore();

 protected:
 	int gameScore;
 	int highScore;
 	int floorHitCount;
 	Ogre::OverlayContainer* overlayContainer;
 	TextOverlay* scoreOverlay;
 	TextOverlay* highScoreOverlay;
 	TextOverlay* gameOverOverlay;
	std::string scoreText;
	Ogre::String scoreLabel;
	std::string highScoreText;
	Ogre::String highScoreLabel;
	Ogre::String gameOverText;
	Ogre::String gameOverLabel;
	std::fstream highScoreFile;
 };