#include "Application.h"
#include "CoreConfig.h"
#include "OgreText.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"
#include <OgreOverlaySystem.h>
#include <string>

using namespace Ogre;

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
	NameValuePairList params;
	// Initialization
	mRoot = new Root("");

	// load plugins
#ifdef _WIN32
	mRoot->loadPlugin("RenderSystem_GL_d");
#endif
#ifdef __linux__
	mRoot->loadPlugin("RenderSystem_GL");
#endif
	try {
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
		t1 = new Timer();
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
		update();
		dTime = temp;
	}

	return true;
}

// Called once per predefined frame
void Application::update() {
	
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
