#include "ScoreManager.h"


ScoreManager::ScoreManager(void) : gameScore(0), floorHitCount(0) {

	gameScore = 0;
	score = new OgreText();
	score->setPos(0.02f, 0.9f); // Text position, using relative co-ordinates
	score->setCol(1.0f, 1.0f, 1.0f, 1.0f); // Text colour (Red, Green, Blue, Alpha)    
	this->postScore();
}

ScoreManager::~ScoreManager(void) {
}

void ScoreManager::postScore(void) {
	/* Set and display ogreText */
	scoreText = "Score: " + std::to_string(gameScore);
	score->setText(scoreText);
}

void ScoreManager::postHighScore(void) {
	return;
}


void ScoreManager::scorePoints(int points) {
	gameScore += points;
	this->nonFloorHit();
	this->postScore();
}

void ScoreManager::nonFloorHit(void) {
	floorHitCount = 0;
}

// Returns false if the game is over.
bool ScoreManager::floorHit(void) {
	floorHitCount++;
	if ( floorHitCount >= 2 ) {
		this->gameOver();
		return false;
	}
	return true;
}

void ScoreManager::resetScore(void) {
	floorHitCount = 0;
	gameScore = 0;	
	score->setPos(0.02f, 0.9f); // Text position, using relative co-ordinates
	score->setCol(1.0f, 1.0f, 1.0f, 1.0f); // Text colour (Red, Green, Blue, Alpha)    
	this->postScore();
}

void ScoreManager::gameOver() {
	
	std::cout << "Game Over\n";
	/*scoreText = "Game Over!";
	score->setCol(1.0f, 1.0f, 1.0f, 1.0f);
	score->setText(scoreText);
	*/
	// Cant create new OgreText, and can't modify score's text here for some reason?

	MultiPlatformHelper::sleep(1000);
	this->resetScore();


}