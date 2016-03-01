#include "ScoreManager.h"


ScoreManager::ScoreManager(void) : gameScore(0), highScoreFile(), highScore(0), floorHitCount(0), scoreLabel("SCORE_"), scoreText("Score: "), highScoreLabel("HIGH_SCORE_"), highScoreText("High Score: "), gameOverLabel("GAME_OVER_"), gameOverText("Game Over!") {
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

ScoreManager::~ScoreManager(void) {
}

void ScoreManager::postScore(void) {
	// scoreOverlay->showOverlay();
	scoreOverlay->setText(scoreText + std::to_string(gameScore));
}

void ScoreManager::postHighScore(void) {
	// highScoreOverlay->showOverlay();
	highScoreOverlay->setText(highScoreText + std::to_string(highScore));
}


void ScoreManager::scorePoints(int points) {
	gameScore += points;
	nonFloorHit();
	postScore();
}

void ScoreManager::nonFloorHit(void) {
	floorHitCount = 0;
}

// Returns false if the game is over.
bool ScoreManager::floorHit(void) {
	floorHitCount++;
	if ( floorHitCount >= 2 ) {
		gameOver();
		return false;
	}
	return true;
}

void ScoreManager::resetScore(void) {
	if ( gameScore > highScore ) {
		highScore = gameScore;
		writeScore();
		postHighScore();
	}
	floorHitCount = 0;
	gameScore = 0;
	postScore();
}

void ScoreManager::gameOver() {
	if ( gameScore > highScore ) {
		highScore = gameScore;
		writeScore();
		postHighScore();
	}
	std::cout << "Game Over\n";
	MultiPlatformHelper::sleep(1000);
	resetScore();
}

void ScoreManager::writeScore() {
	// Replace old highscore with new one
	highScoreFile.open("highscore.txt", std::ios::out);
	highScoreFile << std::to_string(highScore) + "\n";
	highScoreFile.close();
}