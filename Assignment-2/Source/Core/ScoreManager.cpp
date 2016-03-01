#include "ScoreManager.h"


ScoreManager::ScoreManager(void) : gameScore(0), highScore(0), floorHitCount(0), scoreLabel("SCORE_"), scoreText("Score: "), highScoreLabel("HIGH_SCORE_"), highScoreText("High Score: "), gameOverLabel("GAME_OVER_"), gameOverText("Game Over!") {

	scoreOverlay = new TextOverlay(scoreLabel, 0.02f, 0.9f, 0.04f);
	scoreOverlay->setCol(1.0f, 1.0f, 1.0f, 1.0f);
	highScoreOverlay = new TextOverlay(highScoreLabel, 0.67f, 0.9f, 0.04f);
	highScoreOverlay->setCol(1.0f, 1.0f, 1.0f, 1.0f);
}

ScoreManager::~ScoreManager(void) {
}

void ScoreManager::postScore(void) {
	highScoreOverlay->showOverlay();
	highScoreOverlay->setText(highScoreText + std::to_string(highScore));
	scoreOverlay->showOverlay();
	scoreOverlay->setText(scoreText + std::to_string(gameScore));
}

void ScoreManager::postHighScore(void) {
	highScoreOverlay->showOverlay();
	highScoreOverlay->setText(highScoreText + std::to_string(highScore));
}


void ScoreManager::scorePoints(int points) {
	gameScore += points;
	this->nonFloorHit();
	this->postScore();

	/* Currently this is the call to postHighScore that actually causes the overlay to be displayed. */
	/* We shouldnt have to call this every time a point is scored....... */
	this->postHighScore();
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
	if ( gameScore > highScore ) {
		highScore = gameScore;
		this->postHighScore();
	}
	floorHitCount = 0;
	gameScore = 0;
	/*Line 54 apparantly draws the score for the first frame, yet line 53 doesnt draw the high score. Confusing. */
	//this->postHighScore();
	this->postScore();
}

void ScoreManager::gameOver() {
	if ( gameScore > highScore ) {
		highScore = gameScore;
		this->postHighScore();
	}
	std::cout << "Game Over\n";
	// Overlay generated on the fly wont display, yet...
	/*gameOverOverlay = new TextOverlay(gameOverLabel, 0.4f, 0.5f, 5.0f, 5.0f);
	gameOverOverlay->showOverlay();
	gameOverOverlay->setCol(1.0f, 0.0f, 0.0f, 1.0f);
	gameOverOverlay->setText(gameOverText);*/
	MultiPlatformHelper::sleep(1000);
	this->resetScore();
	//delete gameOverOverlay;


}