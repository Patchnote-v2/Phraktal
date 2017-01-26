### TopDown is my experiment
(Last updated 1/25/2017)

The main goals of this project is to hone my programming skills and to learn the necessary tools that often accompany programming, such as git.  It's also an expiriment so see if game development is anything like what I'd like to do.  

Yes, I do understand how absolutely garbage this code is in terms of consistency, readability, structure, performance, and just about any other reasonable coding standard.  I do intend to re-write it eventually, although adding new tacked-on features is always more fun than redoing what already works.

As of now, this has been developed on exclusively on Linux (like that matter so much with SDL2).  In the near future I intend to get SDL2 working on Windows as well.

Current features being added (more or less in order):
- Finish grid-based proximity detection for more effection collision detection (in progress, almost done)
- Enemy spawning and direct-line "pathfinding" (spawning implemented, current have static enemies)
- Level loading and dealing with multiple levels (need to consider what kind of game this is to see if I need this or not)
- Consistent with the needed restructuring of this project, I'd like to make it much easier to add features like new shooting patterns or different enemies, rather than doing what appears to my untrained eye as doing too much in the main loop and not segregating tasks enough.  In due time.

##### Controls:
If the one person who reads this, actually wants to get it to run, and then proceeds to do so, as of writing the controls are:
- WASD to move
- Left Click to fire bullet
- Right Click to teleport
- Q to spawn "enemy" at pointer
- O to spawn an additional random "enemy"
- X to clear all "enemies"