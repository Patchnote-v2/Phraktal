### TopDown is my experiment
(Last updated 2/6/2017)

A recent rewrite has been finished, although now I am wondering about how efficient my grid collision detection is.  That is what I am currently working on. 

The rewrite included the following:
- A global Engine in charge of everything SDL2, such as initialization and the renderer.
- There is now greater distinction between different classes derived from Entity.

As of now, this has been developed on exclusively on Linux (like that matter so much with SDL2).  In the near future I intend to get SDL2 working on Windows as well.

##### Controls:
If the one person who reads this, actually wants to get it to run, and then proceeds to do so, as of writing the controls are:
- WASD to move
- Left Click to fire bullet
- Right Click to teleport
- Q to spawn "enemy" at pointer
- O to spawn an additional random "enemy"
- X to clear all "enemies"