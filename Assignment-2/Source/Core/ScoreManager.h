#pragma once

#include <OgreRoot.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "TextOverlay.h"
#include "MultiPlatformHelper.h"

class ScoreManager {
public:
	ScoreManager(void);
	~ScoreManager(void);

 	Ogre::Real getTime();
 	void addRenderer(CEGUI::OgreRenderer*);
 	void setServer(bool);
 	bool isServer();
 	void postScore(void);
 	void postHighScore(void);
 	void scorePoints(int);
 	void scoreOpponentPoints(int);
 	int getGameScore();
 	int getEnemyScore();
 	void setScore(int points);
 	void setEnemyScore(int points);
 	void updateEnemyPoints(std::string points);
 	void nonFloorHit(void);
 	bool floorHit(void);
 	void resetScore(void);
 	void gameOver(void);
 	void writeScore();
	bool isGameOver();
	void resetGameOver();
	void showGameOver();
	void hideGameOver();

protected:

	// Timing Info
	Ogre::Timer* timer;
	Ogre::Real dt;

	// Score Data
 	int gameScore;
 	int enemyScore;
 	int highScore;
 	int floorHitCount;
 	bool gameOverB;
 	bool server;
 	
	std::string scoreText;
	std::string enemyScoreText;
	Ogre::String scoreLabel;
	std::string highScoreText;
	Ogre::String highScoreLabel;
	Ogre::String gameOverText;
	Ogre::String gameOverLabel;
	std::fstream highScoreFile;

 	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window* gameScoreboard;
    CEGUI::Window* opponentScoreboard;
};