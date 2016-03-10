#include "Application.h"
#include "CoreConfig.h"
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
	mRoot->loadPlugin("Plugin_ParticleFX_d");
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

	// Setup OISManager
    _oisManager = OISManager::getSingletonPtr();
    _oisManager->initialise( mRenderWindow );
    _oisManager->addKeyListener( (OIS::KeyListener*)_oisManager, "keyboardListener" );
    _oisManager->addMouseListener( (OIS::MouseListener*)_oisManager, "mouseListener" );

	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*mRenderWindow);
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setArea( CEGUI::URect( CEGUI::UVector2( CEGUI::UDim( 0.1f, 0 ), CEGUI::UDim( 0.35f, 0 ) ),
								 CEGUI::UVector2( CEGUI::UDim( 0.4f, 0 ), CEGUI::UDim( 0.5f, 0 ) ) ) );
	quit->setText( "Here is a large button.\n(soon) Click to Quit!" );

	sheet->addChild(quit);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	mCamera = mSceneManager->createCamera("Main Camera");
	Ogre::Camera* cam2 = mSceneManager->createCamera("Cam2");
	ballCam = mSceneManager->createCamera("Ball Cam");

	// Add viewport and cameras
	mRenderWindow->addViewport(mCamera);

	mCamera->setAutoAspectRatio(true);
	mCamera->setPosition(0, 120, 1000);

	cam2->setAutoAspectRatio(true);
	cam2->setPosition(1350, 0, -400);
	cam2->yaw(Ogre::Degree(90));
	cam2->pitch(Ogre::Degree(15));

	ballCam->setAutoAspectRatio(true);
	ballCam->setPosition(0, 120, 800);

	cameras = std::vector<Ogre::Camera*>();
	cameras.push_back(mCamera);
	cameras.push_back(cam2);
	cameras.push_back(ballCam);

	Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
	mSceneManager->addRenderQueueListener(pOverlaySystem);

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	_soundScoreManager = new SoundScoreManager();
	_soundScoreManager->startMusic();

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
#ifdef _WIN32
	ResourceGroupManager::getSingleton().addResourceLocation(relative + "/particle", "FileSystem");
#endif
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	t1 = new Timer();

	// Add some light
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* light = mSceneManager->createLight("MainLight");
	light->setCastShadows(true);
	light->setPosition(0, 500, 0);
	light->setType(Ogre::Light::LightTypes::LT_POINT);
	mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	// Test Bullet
	Simulator* mySim = new Simulator();
	_thePaddle = createPaddle("paddle", GameObject::objectType::PADDLE_OBJECT, "paddle.mesh", 0, 0, 0, 100, mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, true, mySim);

	_theBall = createBall("ball", GameObject::objectType::BALL_OBJECT, "tron.mesh", 5, 300, 0, 35, mSceneManager, _soundScoreManager, 1.0f, 1.0f, 0.8f, false, mySim);
	createWall("floor", GameObject::objectType::FLOOR_OBJECT, "floor.mesh", 0, -100, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	createWall("ceiling", GameObject::objectType::WALL_OBJECT, "ceiling.mesh", 0, 600, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(180), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.5f, 0.8f, false, mySim);
	createWall("backwall", GameObject::objectType::BACK_WALL_OBJECT, "backwall.mesh", 0, 300, -1350, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.8f, 0.8f, false, mySim);
	createWall("leftwall", GameObject::objectType::WALL_OBJECT, "leftwall.mesh", 600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	createWall("rightwall", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	createWall("ceiling?", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	createWall("frontwall?", GameObject::objectType::FRONT_WALL_OBJECT, "backwall.mesh", 0, 300, 500, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(180), mSceneManager, _soundScoreManager, 0.0f, 0.9f, 0.8f, false, mySim);

	createRootEntity("stadium", "stadium.mesh", 0, -99, 0);
	mSceneManager->getSceneNode("stadium")->setScale(100,100,100);
	mSceneManager->getSceneNode("stadium")->yaw(Ogre::Degree(90));


	_simulator = mySim;

	_theBall->startScore();
	}
	catch (Exception e) {
		std::cout << "Exception Caught: " << e.what() << std::endl;
	}
}


bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	/* Begun split this into three areas, Setup & GUI, server, client */
	/* This line has been left here for the sake of closing the window. Maybe we'll move it later. */
	if (mRenderWindow->isClosed())
	{
		return false;
	}

	// if ( handleGUI(evt) )
	// 	return true;
	// else if ( !updateServer(evt) ) /* Always returns true for now */
	// 	return false;
	else if ( !updateClient(evt) )
		return false;

    //CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	return true;
}

// Called once per predefined frame
bool Application::update(const FrameEvent &evt) {

	OIS::KeyCode lastKey = _oisManager->lastKeyPressed();

	if(lastKey == OIS::KC_SPACE) {
		_theBall->resetScore();

	}
	else if (lastKey == OIS::KC_M) {
		_soundScoreManager->mute();
	}
	else if (lastKey == OIS::KC_1 || lastKey == OIS::KC_2 || lastKey == OIS::KC_3) {
		int index = lastKey - 2;
		if (index >= 0 && index < cameras.size()) {
			mRenderWindow->removeAllViewports();
			mRenderWindow->addViewport(cameras[index]);
		}
	}
	else if(lastKey == OIS::KC_ESCAPE) {
		// close window when ESC is pressed
		mRunning = false;
	}
		
	ballCam->lookAt(_theBall->getNode()->getPosition());

	// Small pull toward paddle to make it easier for the player to hit the ball
	int pull = 500;
	Ogre::Vector3 paddleAttract = (_thePaddle->getNode()->getPosition() - _theBall->getNode()->getPosition()).normalisedCopy();
	_theBall->applyForce(paddleAttract.x * pull, paddleAttract.y * pull, paddleAttract.z * pull);

	return true;
}

bool Application::handleGUI(const FrameEvent &evt) {

	static bool begin = false;

	if(!begin) {
		_oisManager->capture();

		OIS::KeyCode lastKey = _oisManager->lastKeyPressed();

		if(lastKey == OIS::KC_ESCAPE) {
			// close window when ESC is pressed
			mRunning = false;
			return false;
		}
		return true;
	}
}

bool Application::updateServer(const FrameEvent &evt) {
	return true;
}

/* All logic is now in update client. Update server will be implemented soon. */
bool Application::updateClient(const FrameEvent &evt) {

	static float gameOverTime = 0.0f;
	static float dTime = t1->getMilliseconds();
	
	if (!mRunning)
	{
		return false;
	}
		try {
			_oisManager->capture();
			_thePaddle->movePaddle(_oisManager, height, width);
	}
	catch (Exception e) {

	}

	// Code per frame in fixed FPS
	float temp = t1->getMilliseconds();
	if ((temp - dTime) >= (1.0 / fps)*1000.0) {
		if( _soundScoreManager->isGameOver() ) {
			gameOverTime += (temp - dTime);
		}
		update(evt);
		dTime = temp;
	}

	if ( !(_soundScoreManager->isGameOver()) ) {
		_simulator->stepSimulation(evt.timeSinceLastFrame, 1, 1.0 / fps);
	}
	else {
		// _soundScoreManager->showGameOver();
		if(gameOverTime > 2000) {
			_soundScoreManager->resetGameOver();
			_soundScoreManager->hideGameOver();
			gameOverTime = 0.0f;
		}
	}

	// Constrains the ball's speed
	static int maxSpeed = 4000;
	btVector3 velocity = _theBall->getBody()->getLinearVelocity();
    btScalar speed = velocity.length();
    if(speed > maxSpeed) {
        velocity *= maxSpeed/speed;
        _theBall->getBody()->setLinearVelocity(velocity);
    }

    return true;
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

Ball* Application::createBall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale,scale,scale);
	// ent->setMaterialName("blue");

	Ball* obj = new Ball(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Paddle* Application::createPaddle(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	// sn->showBoundingBox(true);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Paddle* obj = new Paddle(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Wall* Application::createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale.x, scale.y, scale.z);

	sn->pitch(pitch);
	sn->yaw(yaw);
	sn->roll(roll);

	Wall* obj = new Wall(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
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