#include "stickman.h"
#include "level.h"
#include "window.h"
#include <map>

#ifndef GAME_H
#define GAME_H

class Game;

Game* createGame(int argc, char* argv[]);


// Class that handles all interaction between game objects 
// and collision detection
class Game {

private:
	// Game Objects
	GameWindow GW; 
	Background BGround;
	Level      World;	// Randomly generated level
	Sun        TheSun;	// The always moving sun
	Stickman   Player;  // The human controlled player	

	std::vector<Picture*> Drawables;

	bool paused;  	// True iff the game is paused
	bool has_quit;	// Player asked to quit	

	Key facing; // Direction the Player is facing

	int fps; // Frames per second

public:

	// Game is created straight from the command-line args
	Game(int rs, int js, int ss, int fps);

	void handleEvents();
	void sleep();

	// Update the game for the next frame
	void update();
	
	GameWindow& Window() { return GW; };

	// Update the game for when a user does an action
	void press(Key k);

	bool quit();
};




#endif
