#include "window.h"
#include "settings.h"

#ifndef STICKMAN_H
#define STICKMAN_H

enum Action { Standing, Walking };


// A stickman class for drawing the stickman
class Stickman : public Picture {

private:

	// These define the bounding box of the stickman
	int x_min;
	int y_min;

	// The current action the stickman is doing
	Action action;

	int speed;
	bool spread_increase;	// True iff the spread is increasing
	bool blocked;

	// Animate one frame for whatever the current action is
	void continue_walk();

public:
	// Construct a Stickman with a jump speed
	Stickman(int s);
	static Color color;

	void nextFrame();
	
	int static spread;
	
	// Paint the Stickman onto the GameWindow
	void paint(GameWindow& GW);

	// Grab the coordinates of the Stickman
	int max_y();
	int max_x();
	int min_y();
	int min_x();

	Action getAction() { return action; };
	bool is(Action a) { return action == a; };

	void block() { blocked = true; };
	void unblock() { blocked = false; };
	
	// Give the stickman an instruction
	void start(Action instruction);

};


#endif
