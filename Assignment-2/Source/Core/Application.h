#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h>
#include <unordered_map>
#include <string>

class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public Ogre::RenderTargetListener
{
public:
	Application();
	virtual ~Application();

	virtual void init();

	Ogre::Root * mRoot;
	Ogre::RenderWindow * mRenderWindow;
	Ogre::SceneManager * mSceneManager;
	Ogre::Camera * mCamera;
	Ogre::Timer* t1;

	double fps = 300.0;

	bool mRunning = true;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	void createRootEntity(std::string name, std::string mesh, int x, int y, int z);
	void createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z);
	void update();
};
