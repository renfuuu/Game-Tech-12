### Project Description

![Alt text](http://i.imgur.com/wXcjUxh.jpg "A2")

Assignment 2 is building upon assignment 1 where we take out room and ball and add a paddle to play a game. Additions will be Bullet's physics engine, sound effects from the SDL library, a paddle/racquet, and a scoring mechanic. 

#### Room

5-sided room with an invisible mesh infront of the camera.

Three "types" of collision to be handled
* Ball and paddle
* Ball and wall
* Ball escapes through mesh

Velocity loss by collision will be handled with physics engine.

#### Paddle

![Alt text](http://i.imgur.com/tyWzFVS.jpg "Paddle")

The paddle is controlled by the mouse. If the user moves the mouse forward then the paddle moves to up and if the user moves the mouse toward themselves (backwards) then the paddles moves down as illustrated in the left figure. The paddle moves on a parabola left to right by a point at the bottom of the handle as the user moves the mouse left to right.

Collision with the paddle accelerates the ball by a base velocity.

#### Camera Controls

Stationary camera outside the room looking through the invisible mesh.

#### Scoring Mechanism

Increment sore by one point whenever the ball and paddle collide. The game is loss when the ball escapes the room through the invisible mesh or begins to roll (or when the velocity of the ball is reaching zero).