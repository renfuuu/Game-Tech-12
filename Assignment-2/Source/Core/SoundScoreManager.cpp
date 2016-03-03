#include "SoundScoreManager.h"

SoundScoreManager::SoundScoreManager(void) : gameScore(0), highScoreFile(), highScore(0), floorHitCount(0), scoreLabel("SCORE_"), scoreText("Score: "), highScoreLabel("HIGH_SCORE_"), highScoreText("High Score: "), gameOverLabel("GAME_OVER_"), gameOverText("Game Over!") {

	/* Load Sound Resources */

	bool success = true;
	muted = false;
	mutedM = false;
	mutedS = false;

	// Make sure the sounds are initialized to null or your check is useless! (garbage data can be read as !NULL).
	for (int i = 0; i < NUM_SOUNDS; i++) {
		gameSounds[0] = NULL;
	}

	/* Initialize all SDL subsystems */
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		printf( "SDL not initialized! SDL Error: %s\n", Mix_GetError() );
		success = false;
	}

 	/* Initialize SDL_mixer */
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		printf( "SDL_mixer not initialized! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	/* Load music & sound effects */

#ifdef __linux__
	music = Mix_LoadMUS( "../Assets/music_loop.wav" ); 
	gameSounds[PADDLE_BOUNCE] = Mix_LoadWAV("../Assets/ball.wav");
	gameSounds[WALL_BOUNCE] = Mix_LoadWAV("../Assets/wall.wav");
	gameSounds[HEADSHOT] = Mix_LoadWAV("../Assets/headshot.wav");
#endif
#ifdef _WIN32
	music = Mix_LoadMUS( "../../../Game-Tech-12/Assignment-2/Assets/music_loop.wav" ); 
	gameSounds[PADDLE_BOUNCE] = Mix_LoadWAV("../../../Game-Tech-12/Assignment-2/Assets/ball.wav");
	gameSounds[WALL_BOUNCE] = Mix_LoadWAV("../../../Game-Tech-12/Assignment-2/Assets/wall.wav");
	gameSounds[HEADSHOT] = Mix_LoadWAV("../Game-Tech-12/Assignment-2/Assets/headshot.wav");
#endif

	/* Load Score Resources */

	highScoreFile.open("highscore.txt", std::ios::in);

	std::string line;
	while(getline(highScoreFile, line)) {
		if(line == "") {
			highScore = 0;
		}
		else {
			highScore = std::stoi(line);
		}
	}

	highScoreFile.close();
	Ogre::Overlay* ov = Ogre::OverlayManager::getSingletonPtr()->create("Overlay");
	ov->show();

	scoreOverlay = new TextOverlay(ov, scoreLabel, 0.02f, 0.9f, 0.04f);
	scoreOverlay->setCol(1.0f, 1.0f, 1.0f, 1.0f);
	highScoreOverlay = new TextOverlay(ov, highScoreLabel, 0.67f, 0.9f, 0.04f);
	highScoreOverlay->setCol(1.0f, 1.0f, 1.0f, 1.0f);
}

/* Sound Functions */

SoundScoreManager::~SoundScoreManager(void) {
}
 
void SoundScoreManager::startMusic(void) {
	Mix_PlayMusic( music, -1 );
}
/* Play a sound based on the soundID. (Sound IDs are identified in the header) */
void SoundScoreManager::playSound(int soundID) {
	if ( gameSounds[soundID] != NULL && !mutedS ) {
		Mix_PlayChannel( -1, gameSounds[soundID], 0 );
	}
}

// Mute all sounds
void SoundScoreManager::mute(void) {
	muted = !muted;
	mutedM = muted;
	mutedS = muted;

	if ( mutedM != mutedS ) {
		muted = mutedM = mutedS = true;
	}

	muteMusic(mutedM);
}

/* As of now sounds and music
cannot be muted independently. */

// Mute music only
void SoundScoreManager::muteMusic(bool mute) {
	 if ( mute ) {
	 	Mix_PauseMusic();
	 } 
	 else { 
	 	Mix_ResumeMusic();
	 }
}

// Mute sounds only
void SoundScoreManager::muteSounds(bool mute) {
	mutedS = mute;
}

/* Free resources */
void SoundScoreManager::destroy(void) {
	 //Free the music 
	Mix_FreeMusic( music );

	/* Free Sounds */
	for ( int i = 0 ; i < NUM_SOUNDS ; i++ ) {
		if ( gameSounds[i] != NULL )
			Mix_FreeChunk( gameSounds[i] );
		gameSounds[i] = NULL;
	}
	//Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
}

/* Score Functions */

void SoundScoreManager::postScore(void) {
	// scoreOverlay->showOverlay();
	scoreOverlay->setText(scoreText + std::to_string(gameScore));
}

void SoundScoreManager::postHighScore(void) {
	// highScoreOverlay->showOverlay();
	highScoreOverlay->setText(highScoreText + std::to_string(highScore));
}


void SoundScoreManager::scorePoints(int points) {
	gameScore += points;
	nonFloorHit();
	postScore();
}

void SoundScoreManager::nonFloorHit(void) {
	floorHitCount = 0;
}

// Returns false if the game is over.
bool SoundScoreManager::floorHit(void) {
	floorHitCount++;
	if ( floorHitCount >= 2 ) {
		gameOver();
		return false;
	}
	return true;
}

void SoundScoreManager::resetScore(void) {
	if ( gameScore > highScore ) {
		highScore = gameScore;
		writeScore();
		postHighScore();
	}
	floorHitCount = 0;
	gameScore = 0;
	postScore();
}

void SoundScoreManager::gameOver() {
	if ( gameScore > highScore ) {
		highScore = gameScore;
		writeScore();
		postHighScore();
	}
	std::cout << "Game Over\n";
	MultiPlatformHelper::sleep(1000);
	resetScore();
}

void SoundScoreManager::writeScore() {
	// Replace old highscore with new one
	highScoreFile.open("highscore.txt", std::ios::out);
	highScoreFile << std::to_string(highScore) + "\n";
	highScoreFile.close();
}