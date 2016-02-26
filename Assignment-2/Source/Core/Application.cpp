#include "Application.h"
#include "CoreConfig.h"
#include "OgreText.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"
#include <Overlay/OgreOverlaySystem.h>

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <string>
#include "btBulletDynamicsCommon.h"
#include "OISManager.h"
#include "OgreMotionState.h"
#include "Simulator.h"
#include "GameObject.h"

using namespace Ogre;

Application::Application():
points(0)
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
	mRenderWindow = mRoot->createRenderWindow(PROJECT_NAME, width = 800, height = 600, false, &params);
	mCamera = mSceneManager->createCamera("Main Camera");

	Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
	mSceneManager->addRenderQueueListener(pOverlaySystem);

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	// These objects are just to test that we can build bullet and sdl
	btBoxShape* bulletTest = new btBoxShape(btVector3(1, 1, 1));
	sa = new SoundAdapter();
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

#ifdef __linux__
		score = new OgreText();
		score->setPos(0.02f, 0.9f); // Text position, using relative co-ordinates
		score->setCol(1.0f, 1.0f, 1.0f, 1.0f); // Text colour (Red, Green, Blue, Alpha)    
#endif
		// Add viewport
		Viewport * vp = mRenderWindow->addViewport(mCamera);
		mCamera->setAutoAspectRatio(true);
		mCamera->setPosition(0, 120, 1000);
		t1 = new Timer();

		// Add some light
		mSceneManager->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
		mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		Ogre::Light* light = mSceneManager->createLight("MainLight");
		light->setCastShadows(true);
		light->setPosition(0, 500, 0);
		light->setType(Ogre::Light::LightTypes::LT_POINT);
		// light->setDiffuseColour(99.2, 72.2, 7.5);
		// light->setSpecularColour(Ogre::ColourValue::White);
		mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

		// Setup OISManager
	    _oisManager = OISManager::getSingletonPtr();
	    _oisManager->initialise( mRenderWindow );
	    _oisManager->addKeyListener( (OIS::KeyListener*)_oisManager, "keyboardListener" );
	    _oisManager->addMouseListener( (OIS::MouseListener*)_oisManager, "mouseListener" );

		// Test Bullet
		Simulator* mySim = new Simulator();
		GameObject* b1 = createPaddle("test", "paddle.mesh", 0, 0, 0, 100, mSceneManager, 0.0f, 1.0f, 0.8f, true, mySim);

		_thePaddle = b1;

		GameObject* b2 = createBall("test2", "sphere.mesh", _thePaddle, 5, 300, 0, .35, mSceneManager, 1.0f, 1.0f, 0.8f, false, mySim);
		GameObject* b3 = createWall("test3", "floor.mesh", 0, -100, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, 0.0f, 1.0f, 0.8f, false, mySim);
		GameObject* b4 = createWall("test4", "ceiling.mesh", 0, 600, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(180), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, 0.0f, 0.3f, 0.8f, false, mySim);
		GameObject* b5 = createWall("test5", "backwall.mesh", 0, 300, -1350, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, 0.0f, 0.8f, 0.8f, false, mySim);
		GameObject* b6 = createWall("test6", "leftwall.mesh", 600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90), mSceneManager, 0.0f, 1.0f, 0.8f, false, mySim);
		GameObject* b7 = createWall("test7", "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, 0.0f, 1.0f, 0.8f, false, mySim);
		GameObject* b8 = createWall("test8", "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, 0.0f, 1.0f, 0.8f, false, mySim);
		GameObject* b9 = createWall("test9", "backwall.mesh", 0, 300, 500, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(180), mSceneManager, 0.0f, 0.8f, 0.8f, false, mySim);

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
		try {
			_oisManager->capture();
			_thePaddle->movePaddle(_oisManager, height, width);

		// close window when ESC is pressed
		if(_oisManager->getKeyPressed() == OIS::KC_ESCAPE)
			mRunning = false;
	}
	catch (Exception e) {

	}
	// Code per frame in fixed FPS
	float temp = t1->getMilliseconds();
	if ((temp - dTime) >= (1.0 / fps)*1000.0) {
		update(evt);
		dTime = temp;
	}

	_simulator->stepSimulation(evt.timeSinceLastFrame, 50, 1 / fps);

	// Constrains the ball's speed
	static int maxSpeed = 3500;
	btVector3 velocity = _theBall->getBody()->getLinearVelocity();
    btScalar speed = velocity.length();
    if(speed > maxSpeed) {
        velocity *= maxSpeed/speed;
        _theBall->getBody()->setLinearVelocity(velocity);
    }

	return true;
}

// Called once per predefined frame
void Application::update(const FrameEvent &evt) {
	points = _theBall->getPoints();	
#ifdef __linux__
	score->setText("Score: " + std::to_string(points));
#endif

	if(_oisManager->lastKeyPressed() == OIS::KC_SPACE) {
		_theBall->reset();
		points = 0;
		_theBall->setPoints(0);
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

Ball* Application::createBall(Ogre::String nme, Ogre::String meshName, GameObject* paddle, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale,scale,scale);
	ent->setMaterialName("blue");

	Ball* obj = new Ball(nme, paddle->getName(), mSceneManager, sn, ent, ms, mySim, sa, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Paddle* Application::createPaddle(Ogre::String nme, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	// sn->showBoundingBox(true);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Paddle* obj = new Paddle(nme, mSceneManager, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();


	return obj;
}

Wall* Application::createWall(Ogre::String nme, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale.x, scale.y, scale.z);

	sn->pitch(pitch);
	sn->yaw(yaw);
	sn->roll(roll);

	Wall* obj = new Wall(nme, mSceneManager, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
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