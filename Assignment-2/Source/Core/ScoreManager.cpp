#include "ScoreManager.h"

ScoreManager::ScoreManager(void) : gameScore(0), enemyScore(0), highScoreFile(), highScore(0), gameOverB(false), floorHitCount(0), scoreLabel("SCORE_"), scoreText("Red: "), enemyScoreText("Blue: "), highScoreLabel("HIGH_SCORE_"), highScoreText("High Score: "), gameOverLabel("GAME_OVER_"), gameOverText("Game Over!") {
	/* The global timer */
	timer = new Ogre::Timer();
	dt = timer->getMilliseconds();

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

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();

	gameScoreboard = wmgr.createWindow("AlfiskoSkin/Label", "Red");
	gameScoreboard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.92f, 0)),
	CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(1, 0))));

	gameScoreboard->setText("Red: 0");

	opponentScoreboard = wmgr.createWindow("AlfiskoSkin/Label", "Blue");
	opponentScoreboard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.92f, 0)),
	CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0))));
	opponentScoreboard->setText("Blue: 0");

	sheet->addChild(gameScoreboard);
	sheet->addChild(opponentScoreboard);
}

ScoreManager::~ScoreManager(void) {
}

void ScoreManager::addRenderer(CEGUI::OgreRenderer* rend) {
	mRenderer = rend;
}

Ogre::Real ScoreManager::getTime() {
	return timer->getMilliseconds();
}

/* Score Functions */

void ScoreManager::setServer(bool s) {
	server = s;
}

bool ScoreManager::isServer() {
	return server;
}

void ScoreManager::postScore(void) {
	gameScoreboard->setText(scoreText + std::to_string(gameScore));
	opponentScoreboard->setText(enemyScoreText + std::to_string(enemyScore));
}

void ScoreManager::postHighScore(void) {
}


void ScoreManager::scorePoints(int points) {
	gameScore += points;
	nonFloorHit();
	postScore();
}

void ScoreManager::scoreOpponentPoints(int points) {
	enemyScore += points;
	nonFloorHit();
	postScore();
}

int ScoreManager::getGameScore() {
	return gameScore;
}

int ScoreManager::getEnemyScore() {
	return enemyScore;
}

void ScoreManager::setScore(int points) {
	gameScore = points;
}

void ScoreManager::setEnemyScore(int points) {
	enemyScore = points;
}

void ScoreManager::updateEnemyPoints(std::string points) {
	//Networking received here
	postScore();
}

void ScoreManager::nonFloorHit(void) {
	floorHitCount = 0;
}

/*** REMOVE! ***/
// Returns false if the game is over.
bool ScoreManager::floorHit(void) {
	floorHitCount++;
	if ( floorHitCount >= 2 ) {
		// gameOver();
		// return false;
	}
	return true;
}
/***         ***/

void ScoreManager::resetScore(void) {
	floorHitCount = 0;
	gameScore = 0;
	enemyScore = 0;
	postScore();
}

bool ScoreManager::isGameOver() {
	// If a player has more than 20 points && wins by more than one point return true;
	return ( gameScore > 0 || enemyScore > 0 ) && ( gameScore != enemyScore && Ogre::Math::Abs(gameScore - enemyScore) > 0 );
}

void ScoreManager::resetGameOver() {
	gameOverB = false;
}

void ScoreManager::showGameOver() {
	gameScoreboard->hide();
}

void ScoreManager::hideGameOver() {
	gameScoreboard->hide();
}


void ScoreManager::gameOver() {
	gameOverB = true;
	// gameOverOverlay->setText(gameOverText);
	showGameOver();
	resetScore();
}

void ScoreManager::writeScore() {
	// Replace old highscore with new one
	highScoreFile.open("highscore.txt", std::ios::out);
	highScoreFile << std::to_string(highScore) + "\n";
	highScoreFile.close();
}