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

#ifdef __linux__
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	// This is really just a debugging try-catch block for catching and printing exceptions
	try {
	NameValuePairList params;
	// Initialization
	mRoot = new Root(mPluginsCfg);

#ifdef __linux__
	setupResources();
#endif

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
		ResourceGroupManager::getSingleton().addResourceLocation("../../../Game-Tech-12/Assignment-2/Assets", "FileSystem");
#endif
#ifdef __linux__
		std::string relative = "/lusr/opt/ogre-1.9/share/OGRE/Media";
#endif
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/models", "FileSystem");
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/materials", "FileSystem");
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/materials/textures", "FileSystem");
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/materials/programs/GLSL", "FileSystem");
		ResourceGroupManager::getSingleton().addResourceLocation(relative + "/materials/scripts", "FileSystem");
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// Add viewport
		Viewport * vp = mRenderWindow->addViewport(mCamera);
		mCamera->setAutoAspectRatio(true);
		mCamera->setPosition(0, -350, 600);
		t1 = new Timer();

		// Add some light
		mSceneManager->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
		mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		Ogre::Light* light = mSceneManager->createLight("MainLight");
		light->setCastShadows(true);
		light->setPosition(0, 0, 0);
		light->setType(Ogre::Light::LightTypes::LT_POINT);
		mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

		// Test Bullet
		Simulator* mySim = new Simulator();
		GameObject* b1 = createPaddle("test", "paddle.mesh", 0, -400, 50, 100, mSceneManager, 0.0f, 1.0f, 0.0f, mySim);
		GameObject* b2 = createBall("test2", "sphere.mesh", 5, -100, 0, .2, mSceneManager, 1.0f, 1.0f, 0.0f, mySim);

		_theBall = b2;

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
		// Example of how to do a direct translation of a gameobject
		// _theBall->translate(0, 2.0, 0);
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

Ball* Application::createBall(Ogre::String nme, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale,scale,scale);

	Ball* obj = new Ball(nme, mSceneManager, sn, ent, ms, mySim, mss, rest, frict, scale);
	obj->addToSimulator();

	return obj;
}

Paddle* Application::createPaddle(Ogre::String nme, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	// sn->showBoundingBox(true);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Paddle* obj = new Paddle(nme, mSceneManager, sn, ent, ms, mySim, mss, rest, frict, scale);
	obj->addToSimulator();


	return obj;
}

void Application::setupResources(void){
	// Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
	#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}