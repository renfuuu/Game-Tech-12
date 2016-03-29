#include "btBulletDynamicsCommon.h"
#include <OgreEntity.h>
#include <OgreRoot.h>

class GameState {
public:
	Ogre::Quaternion _thePaddle;
	Ogre::Quaternion _otherPaddle;
	Ogre::Vector3 _thePaddlePos;
	Ogre::Vector3 _otherPaddlePos;
	Ogre::Vector3 _ballPos;
	btVector3 _velocity;

	GameState(Ogre::Quaternion qt1, Ogre::Quaternion qt2, Ogre::Vector3 pPos, Ogre::Vector3 oPos, Ogre::Vector3 pos, btVector3 vel) : _thePaddle(qt1), _otherPaddle(qt2), _thePaddlePos(pPos), _otherPaddlePos(oPos), _ballPos(pos), _velocity(vel) {}
};