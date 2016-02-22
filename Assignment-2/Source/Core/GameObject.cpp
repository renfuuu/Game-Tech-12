#include "GameObject.h"
#include <btBulletDynamicsCommon.h>

//Add the game object to the simulator
void GameObject::addToSimulator() {
	simulator->addObject(this);
}