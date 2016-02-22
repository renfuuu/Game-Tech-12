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

Increment sore by one point whenever the ball and paddle collide. The game is loss when the ball escapes the room through the invisible mesh or begins to roll (or when the velocity of the ball is reaching zero). Game win is to be determined.

#### Architecture

##### Application Layer

Contains the scene, set up, and gameloop. The Scene has a ball and paddle entity, soundadapter, and a scoremanager. Inside the gameloop we will have an update function that uses a global timer to modify the frames per second to 60 fps. We'll see getSingleton to use the Input Event Manager

##### Sound Adapter

A class that will handle our sounds when called.

Types of sounds:
* Ball collides with wall
* Ball collides with paddle
* Player loses
* Player wins

##### Score Manager

A class that keeps track of points.

##### Input Manager 

A class to handle input from the player.

#### Division of Labor
* David: Bullet, Input Event Manager, Application
* Frank: SDL, Sound Adapter, Application
* Itoro: Doc, Score Manager, Application
