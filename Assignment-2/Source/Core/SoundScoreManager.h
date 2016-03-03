#pragma once

#ifdef _WIN32
#include <SDL.h>
#include <SDL_mixer.h>
#endif
#ifdef __linux__
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#endif

#include <OgreRoot.h>

#include "TextOverlay.h"
#include "MultiPlatformHelper.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

class SoundScoreManager {
public:
	enum { WALL_BOUNCE, PADDLE_BOUNCE, GAME_WIN, GAME_LOSS, HEADSHOT, NUM_SOUNDS } sounds;

	SoundScoreManager(void);
	~SoundScoreManager(void);

 	void setDT();
 	void setDT(Ogre::Real last);
 	Ogre::Real getTime();
 	Ogre::Real getDT();

	void playSound(int);
	void startMusic(void);
	void mute(void);
	void muteMusic(bool mute);
	void muteSounds(bool mute);
	void destroy(void);

 	void postScore(void);
 	void postHighScore(void);
 	void scorePoints(int);
 	void nonFloorHit(void);
 	bool floorHit(void);
 	void resetScore(void);
 	void gameOver(void);
 	void writeScore();

private:

	// Timing Info
	Ogre::Timer* timer;
	Ogre::Real dt;

	// Sound Data
	bool muted; //overall muted
	bool mutedM; //music
	bool mutedS; //sounds
	Mix_Music *music = NULL;
	Mix_Chunk *gameSounds[NUM_SOUNDS];


	// Score Data
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