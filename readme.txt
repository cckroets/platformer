          ___  __ 
    /\   / _ \/_ |
   /  \ | | | || |
  / /\ \| | | || |
 / ____ \ |_| || |
/_/    \_\___/ |_|


Pencil Platformer
------------------
Pencil Platformer is a side-scrolling game made in C/C++ using Xlib. This 
game was designed for CS 349, A01, at the University of Waterloo, Fall 2013.

The aim of the game is to get to the end of the level, still alive, while 
gathering as many Coins as possible. There are many obstacles that must
be avoided to win. 

Pencil Crayons: These colorful, upright pencils will stab the player and paint
him the same colour as the crayon. Standing on one of these sharp crayons will 
slowly drain health points.

Sharp Pencils: Sharp pencils will also drain HP, in addition to shoot a bullet 
upwards. Timing must be considered when dealing with sharp pencils. Making contact 
with a bullet with take away 10 health points! 

Each game is a completely new level, for unlimited fun!



Enhancements
------------

1.) Enemies & HP: Instead of creating a Coin with a negative value, I have introduced
a new metric Health Points (HP) that decreases when the player makes contact with
an enemy. The shooting enemies shoot one bullet straight up until it exits the
screen, then it fires another.

2.) Graphics: There is an obvious theme, of office supplies for every game object. Each
graphic is completely hand-coded, no pictures or bitmaps of any sort are used. 

3.) Randomly Generated: Although this game does not create an infinitely long level, it 
easily could. The level is randomly-generated, but does have a cut-off point so that the 
player can actually win. Having the player be able to both win and lose in the game was
very important to me. Without some sort of victory the game seems very unfinished.  



Compile and run:
----------------
$ make
$ ./game [RUN_SPEED JUMP_SPEED SUN_SPEED FPS] 



Command-Line arguments
----------------------
The game will run normally with no arguments selected,
but if one is provided, all must be provided.

All speeds must be an integer between 0-10. The default setting is 5.
The higher the number the faster that speed is.

Frames per second must be an integer between 1-100. Default is 30.
Use 40 for a very smooth experience.

Note that some configurations may appear glitchy, e.g. very, very, high speed
with a very low framerate. 



