#include "Application.h"
#include "CoreConfig.h"
#include "OgreText.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"
#include <Overlay/OgreOverlaySystem.h>
#include <string>
#include "btBulletDynamicsCommon.h"
#include "SoundAdapter.h"
#include "OgreMotionState.h"
#include "Simulator.h"
#include "GameObject.h"

using namespace Ogre;

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
	// This is really just a debugging try-catch block for catching and printing exceptions
	try {
	NameValuePairList params;
	// Initialization
	mRoot = new Root("");

	// load plugins
#ifdef _WIN32
	mRoot->loadPlugin("RenderSystem_GL_d");
#endif
#ifdef __linux__
	mRoot->loadPlugin("/lusr/opt/ogre-1.9/lib/OGRE/RenderSystem_GL");
#endif

	// Select render system
	const RenderSystemList &renderers = mRoot->getAvailableRenderers();
	RenderSystem * renderSystem = nullptr;
	LogManager::getSingletonPtr()->logMessage("Getting available renderers");
	for (auto renderer = renderers.begin(); renderer != renderers.end(); renderer++)
	{
		String name = (*renderer)->getName();
		LogManager::getSingletonPtr()->logMessage(name);
		renderSystem = *renderer;
	}
	if (renderSystem)
	{
		LogManager::getSingletonPtr()->logMessage("Using renderer " + renderSystem->getName());
		mRoot->setRenderSystem(renderSystem);
	}
	else
	{
		LogManager::getSingletonPtr()->logMessage(LML_CRITICAL, "Initializing render system failed. No renderers available.");
	}

	// Initialize with render system, no new window (yet)
	mRoot->initialise(false);

	// Create scene manager, render window, and camera
	mSceneManager = mRoot->createSceneManager(ST_GENERIC);
	mRenderWindow = mRoot->createRenderWindow(PROJECT_NAME, 800, 600, false, &params);
	mCamera = mSceneManager->createCamera("Main Camera");

	Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
	mSceneManager->addRenderQueueListener(pOverlaySystem);

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	// These objects are just to test that we can build bullet and sdl
	btBoxShape* bulletTest = new btBoxShape(btVector3(1, 1, 1));
	SoundAdapter* soundTest = new SoundAdapter();
	// GameObject* obj = new GameObject();


#ifdef _WIN32
		std::string relative = "../../../ogre/build/sdk/media";
#endif
#ifdef __linux__
		std::string relative = "/lusr/opt/ogre-1.9/share/OGRE/Media";
#endif
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/models", "FileSystem");
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/materials", "FileSystem");
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/materials/textures", "FileSystem");
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// Add viewport
		Viewport * vp = mRenderWindow->addViewport(mCamera);
		mCamera->setAutoAspectRatio(true);
		mCamera->setPosition(0, -300, 600);
		t1 = new Timer();

		// Add some light
		mSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
		mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		Ogre::Light* light = mSceneManager->createLight("MainLight");
		light->setCastShadows(true);
		light->setPosition(0, 0, 0);
		light->setType(Ogre::Light::LightTypes::LT_POINT);

		// Test Bullet
		Simulator* mySim = new Simulator();
		createRootEntity("test", "sphere.mesh", 0, -100, 0);
		Ogre::SceneNode* sn = mSceneManager->getSceneNode("test");
		Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, "test", 0);
		const btTransform pos;
		OgreMotionState* ms = new OgreMotionState(pos, sn);

		GameObject* obj = new GameObject("test", mSceneManager, sn, ent, ms, mySim, 1.0f);
		obj->addToSimulator();

		// TODO make a createGameObject() method
		createRootEntity("test2", "sphere.mesh", 0, -400, 0);
		Ogre::SceneNode* sn1 = mSceneManager->getSceneNode("test2");
		Ogre::Entity* ent1 = SceneHelper::getEntity(mSceneManager, "test2", 0);
		const btTransform pos1;
		OgreMotionState* ms1 = new OgreMotionState(pos1, sn1);

		GameObject* obj1 = new GameObject("test", mSceneManager, sn1, ent1, ms1, mySim, 0.0f);
		obj1->addToSimulator();

		_simulator = mySim;
	}
	catch (Exception e) {
		std::cout << "Exception Caught: " << e.what() << std::endl;
	}
}


bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	static float dTime = t1->getMilliseconds();
	
	if (mRenderWindow->isClosed())
	{
		return false;
	}

	if (!mRunning)
	{
		return false;
	}

	// Code per frame in fixed FPS
	float temp = t1->getMilliseconds();
	if ((temp - dTime) >= (1.0 / fps)*1000.0) {
		update(evt);
		dTime = temp;
	}

	_simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1 / fps);

	return true;
}

// Called once per predefined frame
void Application::update(const FrameEvent &evt) {
	
	try{
		
	}
	catch (Exception e) {

	}
}


void Application::createRootEntity(std::string name, std::string mesh, int x, int y, int z) {
	Ogre::Entity* ogreEntity = mSceneManager->createEntity(name, mesh);
	ogreEntity->setCastShadows(true);
	Ogre::SceneNode* ogreNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(ogreEntity);
	ogreNode->setPosition(x, y, z);
}

void Application::createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z) {
	Ogre::Entity* ogreEntity = mSceneManager->createEntity(name, mesh);
	ogreEntity->setCastShadows(true);
	Ogre::SceneNode* ogreNode = sceneNode->createChildSceneNode(name);
	ogreNode->attachObject(ogreEntity);
	ogreNode->setPosition(x, y, z);
}
