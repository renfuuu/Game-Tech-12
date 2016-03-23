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
	// This is really just a debugging try-catch block for catching and printing exceptions
	try {

		t1 = new Timer();

		_simulator = new Simulator();

		setupWindowRendererSystem();

		setupOIS();

		setupCEGUI();

		setupCameras();

		setupSSM();

		loadResources();

		setupLighting();

		createObjects();

	}
	catch (Exception e) {
		std::cout << "Exception Caught: " << e.what() << std::endl;
	}
}


/* 
* Update Methods 
*/
bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	/* Begun split this into three areas, Setup & GUI, server, client */
	/* This line has been left here for the sake of closing the window. Maybe we'll move it later. */
	if (mRenderWindow->isClosed())
	{
		return false;
	}

	if ( handleGUI(evt) )
		return true;
	else if ( server && !updateServer(evt) ) /* Always returns true for now */
		return false;
	else if ( !updateClient(evt) )
		return false;

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

	if ( !(_soundScoreManager->isGameOver()) ) {
		_simulator->stepSimulation(evt.timeSinceLastFrame, 1, 1.0 / fps);
	}
	_thePaddle->movePaddle(_oisManager, height, width);

	// Small pull toward paddle to make it easier for the player to hit the ball
	int pull = 500;
	Ogre::Vector3 paddleAttract = (_thePaddle->getNode()->getPosition() - _theBall->getNode()->getPosition()).normalisedCopy();
	_theBall->applyForce(paddleAttract.x * pull, paddleAttract.y * pull, paddleAttract.z * pull);

	std::string t = "Test";
	if ( !server ) {
		if(t.length() - 1 > NetManager::MESSAGE_LENGTH) {
			std::cout << "Message was too large." << std::endl;
			return error();
		}
		netManager->messageServer(PROTOCOL_UDP, t.c_str(), t.length() + 1);
	}

	return true;
}

bool Application::handleGUI(const FrameEvent &evt) {

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
	if ( netManager->pollForActivity(1) ) {
		std::unordered_map<std::string, char*> pairs = dataParser(netManager->udpServerData[0].output);

		// for(auto& var : pairs) {
		// 	std::cout << var.first << " : " << var.second << std::endl;
		// }

		float w = atof(pairs["PDW"]);
		float x = atof(pairs["PDX"]);
		float y = atof(pairs["PDY"]);
		float z = atof(pairs["PDZ"]);
		float paddleX = atof(pairs["PPX"]);
		float paddleY = atof(pairs["PPY"]);
		float paddleZ = atof(pairs["PPZ"]);

		Ogre::Quaternion qt(w,x,y,z);
		_otherPaddle->setOrientation(qt);
		_otherPaddle->setPosition(paddleX, paddleY, paddleZ - 1000);
		_otherPaddle->roll(Ogre::Degree(180));
	}
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

	if(_soundScoreManager->isGameOver()) {
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

	ballCam->lookAt(_theBall->getNode()->getPosition());

    return true;
}
/* 
* End Update Methods 
*/


/* 
* Create Object Methods 
*/
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
	ent->setMaterialName("blue");

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
/*
* End Create Object Methods 
*/


/* 
* Initialization Methods
*/
void Application::setupWindowRendererSystem(void) {

#ifdef __linux__
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

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
	mRenderWindow = mRoot->createRenderWindow(PROJECT_NAME, width = 1200, height = 900, false, &params);
}

void Application::setupResources(void) {
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


void Application::setupOIS(void) {

    _oisManager = OISManager::getSingletonPtr();
    _oisManager->initialise( mRenderWindow );
    _oisManager->addKeyListener( (OIS::KeyListener*)_oisManager, "keyboardListener" );
    _oisManager->addMouseListener( (OIS::MouseListener*)_oisManager, "mouseListener" );

}

void Application::setupCEGUI(void) {

	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*mRenderWindow);
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("AlfiskoSkin/MouseArrow");

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "_MasterRoot");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	CEGUI::Window* quitButton = wmgr.createWindow("AlfiskoSkin/Button", "QuitButton");
	quitButton->setArea( CEGUI::URect( CEGUI::UVector2( CEGUI::UDim( 0.0f, 0 ), CEGUI::UDim( 0.0f, 0 ) ),
								 CEGUI::UVector2( CEGUI::UDim( 0.1f, 0 ), CEGUI::UDim( 0.05f, 0 ) ) ) );
	quitButton->setText( "Quit" );

	hostServerButton = wmgr.createWindow("AlfiskoSkin/Button", "HostButton");
	hostServerButton->setArea( CEGUI::URect( CEGUI::UVector2( CEGUI::UDim( 0.3f, 0 ), CEGUI::UDim( 0.35f, 0 ) ),
								 CEGUI::UVector2( CEGUI::UDim( 0.7f, 0 ), CEGUI::UDim( 0.4f, 0 ) ) ) );
	hostServerButton->setText( "Host Game" );

	joinServerButton = wmgr.createWindow("AlfiskoSkin/Button", "JoinButton");
	joinServerButton->setArea( CEGUI::URect( CEGUI::UVector2( CEGUI::UDim( 0.3f, 0 ), CEGUI::UDim( 0.4f, 0 ) ),
								 CEGUI::UVector2( CEGUI::UDim( 0.7f, 0 ), CEGUI::UDim( 0.45f, 0 ) ) ) );
	joinServerButton->setText( "Join Game" );

	sheet->addChild(hostServerButton);
	sheet->addChild(joinServerButton);
	sheet->addChild(quitButton);

	hostServerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::StartServer, this));
	joinServerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::JoinServer, this));
	quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::Quit, this));
}

void Application::setupCameras(void) {

	mCamera = mSceneManager->createCamera("Main Camera");
	Ogre::Camera* cam2 = mSceneManager->createCamera("Cam2");
	ballCam = mSceneManager->createCamera("Ball Cam");

	// Add viewport and cameras
	mRenderWindow->addViewport(mCamera);

	mCamera->setAutoAspectRatio(true);
	mCamera->setPosition(0, 120, 1050);
	mCamera->pitch(Ogre::Degree(10));

	cam2->setAutoAspectRatio(true);
	cam2->setPosition(1350, 0, -400);
	cam2->yaw(Ogre::Degree(90));
	cam2->pitch(Ogre::Degree(15));

	ballCam->setAutoAspectRatio(true);
	ballCam->setPosition(0, 120, 1050);

	cameras = std::vector<Ogre::Camera*>();
	cameras.push_back(mCamera);
	cameras.push_back(cam2);
	cameras.push_back(ballCam);

}

/* Setup SoundScoreManager */
void Application::setupSSM(void) {

	Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
	mSceneManager->addRenderQueueListener(pOverlaySystem);

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	_soundScoreManager = new SoundScoreManager();
	_soundScoreManager->startMusic();

}

void Application::loadResources(void) {

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

}

void Application::setupLighting(void) {

	mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* directionalLight = mSceneManager->createLight("Sun");
	directionalLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	directionalLight->setCastShadows(true);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.8, .8, .8));
	directionalLight->setSpecularColour(Ogre::ColourValue(.8, .8, .8));

	directionalLight->setDirection(Ogre::Vector3(0, -1, .1));

}

void Application::createObjects(void) {
	
	mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	_thePaddle = createPaddle("paddle", GameObject::objectType::PADDLE_OBJECT, "paddle.mesh", 0, 0, 0, 100, mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, true, _simulator);
	_otherPaddle = createPaddle("other_paddle", GameObject::objectType::PADDLE_OBJECT, "paddle.mesh", 0, 0, -1000, 100, mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, true, _simulator);
	_theBall = createBall("ball", GameObject::objectType::BALL_OBJECT, "sphere.mesh", 5, 300, 0, .35, mSceneManager, _soundScoreManager, 1.0f, 1.0f, 0.8f, false, _simulator);

	createWall("floor", GameObject::objectType::FLOOR_OBJECT, "floor.mesh", 0, -100, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("ceiling", GameObject::objectType::WALL_OBJECT, "ceiling.mesh", 0, 600, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(180), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.5f, 0.8f, false, _simulator);
	createWall("backwall", GameObject::objectType::BACK_WALL_OBJECT, "backwall.mesh", 0, 300, -1350, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.8f, 0.8f, false, _simulator);
	createWall("leftwall", GameObject::objectType::WALL_OBJECT, "leftwall.mesh", 600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("rightwall", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("ceiling?", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("frontwall?", GameObject::objectType::FRONT_WALL_OBJECT, "backwall.mesh", 0, 300, 500, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(180), mSceneManager, _soundScoreManager, 0.0f, 0.9f, 0.8f, false, _simulator);

	createRootEntity("stadium", "stadium2.mesh", 0, -592, 0);
	mSceneManager->getSceneNode("stadium")->setScale(100,100,100);
	mSceneManager->getSceneNode("stadium")->yaw(Ogre::Degree(90));

	_theBall->startScore();

}
/* 
* End Initialization Methods
*/


/* 
*CEGUI Button Callbacks 
*/
bool Application::StartServer(const CEGUI::EventArgs& e) {

	begin = true;
	server = true;

	if(!setupNetwork(server)) {
		return error();
	}
	else {
		hostServerButton->hide();
		joinServerButton->hide();
		return true;
	}
}

bool Application::JoinServer(const CEGUI::EventArgs& e) {

	begin = true;
	server = false;

	if(!setupNetwork(server)) {
		return error();
	}
	else {
		hostServerButton->hide();
		joinServerButton->hide();
		return true;
	}
}

bool Application::Quit(const CEGUI::EventArgs& e) {

	mRunning = false;
    return true;
}

bool Application::setupNetwork(bool isServer) {

	netManager = new NetManager();

	// char buf[512] = "t1 2312\nt2 23.342\nt3 hello";
	// dataParser(buf);
	// error();

	if(!netManager->initNetManager()) {
		std::cout << "Failed to init the server!" << std::endl;
		return false;
	}
	else {
		// Opens a connection on port 51215
		netManager->addNetworkInfo(PROTOCOL_UDP, isServer ? NULL : "128.83.120.177", 51215);
	}

	if(isServer) {
		if(!netManager->startServer()) {
			std::cout << "Failed to start the server!" << std::endl;
			return false;
		}
		return true;
	}
	else {
		if(!netManager->startClient()) {
			std::cout << "Failed to start the client!" << std::endl;
			return false;
		}
		return true;
	}

}

bool Application::error() {
	mRunning = false;
	return false;
}

std::unordered_map<std::string, char*> Application::dataParser(char* buf) {
	std::unordered_map<std::string, char*> kvpairs;
    char *end_str;
    char *token = strtok_r(buf, "\n", &end_str);

    while (token != NULL)
    {
        char *end_token;
        char *token2 = strtok_r(token, " ", &end_token);
        std::vector<char*> info;
        while (token2 != NULL)
        {
    		info.push_back(token2);
            token2 = strtok_r(NULL, " ", &end_token);
        }

        char* key = info[0];
        char* value = info[1];
        kvpairs[std::string(key)] = value;

        token = strtok_r(NULL, "\n", &end_str);
    }

    return kvpairs;
}
