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

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <unordered_map>
#include <string>

#include "SoundScoreManager.h"
#include "OISManager.h"
#include "Simulator.h"
#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"

class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public Ogre::RenderTargetListener
{
public:
	Application();
	virtual ~Application();

	virtual void init();

	Ogre::Root * mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow * mRenderWindow;
	Ogre::SceneManager * mSceneManager;
	Ogre::Camera * mCamera;
	Ogre::Camera * ballCam;
	Ogre::Timer* t1;

	SoundScoreManager* _soundScoreManager;
	OISManager* _oisManager;
	Simulator* _simulator;
	GameObject* _theBall;
	GameObject* _thePaddle;

    CEGUI::OgreRenderer* mRenderer;
	
	std::vector<Ogre::Camera*> cameras;

	int points;
	int width;
	int height;

	double fps = 300.0;

	bool begin = false;
	bool mRunning = true;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	void createRootEntity(std::string name, std::string mesh, int x, int y, int z);
	void createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z);
	bool update(const Ogre::FrameEvent &evt);
	bool handleGUI(const Ogre::FrameEvent &evt);
	bool updateServer(const Ogre::FrameEvent &evt);
	bool updateClient(const Ogre::FrameEvent &evt);

	void movePaddle();
	Ball* createBall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Paddle* createPaddle(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Wall* createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	void setupWindowRendererSystem(void);
	void setupResources(void);
	void setupOIS(void);
	void setupCEGUI(void);
	void setupCameras(void);
	void setupSSM(void);
	void loadResources(void);
	void setupLighting(void);
	void createObjects(void);
	bool startServer(const CEGUI::EventArgs &e);
	bool quit(const CEGUI::EventArgs &e);
};
