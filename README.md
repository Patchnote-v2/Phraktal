### TopDown is my experiment
(Last updated 1/26/2017)

The main goals of this project is to hone my programming skills and to learn the necessary tools that often accompany programming, such as git.  It's also an expiriment so see if game development is anything like what I'd like to do.  

I am currently in the process of rewriting the majority of the code so as to better structure it and allow for new features to integrate into it more naturally, rather than being an entirely new thing on its own.  Currently I have the following design:
- A global Engine in charge of everything SDL2, such as initialization and the renderer.  This means the Engine is responsibly for also initializing the texture for each Entity as well as rendering them.
- There is now greater distinction between different Entities.
- A great reduction in smart pointers has been achieved.  Thankfully literally everything isn't a shared_ptr anymore. 

As of now, this has been developed on exclusively on Linux (like that matter so much with SDL2).  In the near future I intend to get SDL2 working on Windows as well.

Current features being added (more or less in order):
- Finish grid-based proximity detection for more effective collision detection (in progress, almost done)
- Enemy spawning and direct-line "pathfinding" (spawning implemented, current have static enemies)
- Level loading and dealing with multiple levels (need to consider what kind of game this is to see if I need this or not)

##### Controls:
If the one person who reads this, actually wants to get it to run, and then proceeds to do so, as of writing the controls are:
- WASD to move
- Left Click to fire bullet
- Right Click to teleport
- Q to spawn "enemy" at pointer
- O to spawn an additional random "enemy"
- X to clear all "enemies"
- M removes the player from the grid