## Milestone 2

### How to play
* Move the mouse up/down and left/right on the game window to control the paddle
* Strike the ball with the paddle to propel it through the room and increase your score
* Space bar resets score and the ball back to its inital position

### Progress

#### Summary
We feel that we are actually ahead of schedule and everything went smoothly this week and we did not get behind on any of our deliverables.

#### Implemented
* We were able to get gameobjects of types paddle, wall and ball set up and working perfectly.
* Bullet simulator with motionstate wrapper that handles motions between bullet transforms and ogre scene transforms
* Working collisions / collision detection
* User controls are there for the most part especially for the mouse
* Paddle moves on parabolic arc in 3d space that gives a more realsitic feel for the motion of the paddle
* Ball loses velocity over time until the player hits it again with the paddle
* Different sounds played for collision between ball/paddle and ball/wall
* Added scoring system that increases points with each strike of the ball
* Added GUI to display score to the user
* Ball reset

#### Stil Need Implementation
* Camera controls - not yet implemented, need to add an option for user to change his view
* Game polishing - need to smooth out the interaction with the paddle and the paddle to the ball
* Add extra credits - Background music, end condition, particles, user interface GUI
* Muting - mute all volume and sounds

#### C++ Classes Used
* Application - Sets up scene, controls game logic
* Simulator - Holds a list of gameobjects, steps them inside the bullet dynamic world
* BulletContactCallback - Bullet calls this class when a collision between two objects is detected
* CollisionContext - Stores information about the collision that is passed through the contact callback
* OgreMotionState - Updates transforms between Ogre world and bullet world
* GameObject - A wrapper for storing bullet/ogre information and methods to abstract their usage
* Paddle - Child of GameObject, adds a collider shape around a paddle mesh
* Ball - Child of GameObject, adds a collider sphere shape around a ball mesh
* Wall - Child of GameObject, adds a cube collider shape around a plane mesh
* OISManager - Event driven user input detection class
* SoundAdapter - Stores sounds and plays them into the ogre scene
* OgreText - Quick and dirty text overlay for Ogre
* SceneHelper - Allows us to get information faster from scennodes to entities
* MultiPlatFormHelper - Abstract usages for things like timing and printing on different platforms

### Code and Outside resources used
* Paddle model downloaded from: http://www.blendswap.com/blends/view/49333 credit: Bruno Matheus
* Started with some of the bullet example code from the lecture slides
* Sound files from the public domain
* OgreText - Copied from http://ogre3d.org/tikiwiki/tiki-index.php?page=Simple+text


## Milestone 1

### Project Description

#### Overall Design

![Alt text](http://i.imgur.com/wXcjUxh.jpg "A2")

Assignment 2 is building upon assignment 1 where we take out room and ball and add a paddle to play a game. Additions will be Bullet's physics engine, sound effects from the SDL library, a paddle/racquet, and a scoring mechanic. 

##### Room

5-sided room with an invisible mesh in front of the camera.

Three "types" of collision to be handled
* Ball and paddle
* Ball and wall
* Ball escapes through mesh

Velocity loss by collision will be handled with physics engine.

##### Paddle

![Alt text](http://i.imgur.com/tyWzFVS.jpg "Paddle")

The paddle is controlled by the mouse. If the user moves the mouse forward then the paddle moves to up and if the user moves the mouse toward themselves (backwards) then the paddles moves down as illustrated in the left figure. The paddle moves on a parabola left to right by a point at the bottom of the handle as the user moves the mouse left to right.

Collision with the paddle accelerates the ball by a base velocity.

##### Camera Controls

Stationary camera outside the room looking through the invisible mesh.

##### Scoring Mechanism

Increment sore by one point whenever the ball and paddle collide. The game is loss when the ball bounces twice on the ground. After a game loss, highscore is recorded and game loss music plays.

#### Architecture

##### Application Layer

Contains the scene, set up, and gameloop. The Scene has a ball and paddle entity, soundadapter, and a scoremanager. Inside the gameloop we will have an update function that uses a global timer to modify the frames per second to 60 fps. We'll see getSingleton to use the Input Event Manager

##### Sound Adapter

A class that will handle our sounds when called.

Types of sounds:
* Ball collides with wall
* Ball collides with paddle
* Player loses

The player is able to mute the background music by pressing M.

##### Score Manager

A class that keeps track of points.

##### Input Manager 

A class to handle input from the player.

#### Division of Labor
* David: Bullet, Input Event Manager, Application
* Frank: SDL, Sound Adapter, Application
* Itoro: Doc, Score Manager, Application

#### Credit:
* Paddle model downloaded from: http://www.blendswap.com/blends/view/49333 credit: Bruno Matheus
