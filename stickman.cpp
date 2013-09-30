#include "stickman.h"
#include <cstdlib>
#include <iostream>

// Calculate the y-value for the peak of a jump 
int calc_peak(int min_y) {
	return min_y - JUMP_HEIGHT;
}

int Stickman::spread = MAN_SPREAD/2;

Stickman::Stickman(int s) : 
	speed(std::max(1,s-1)), blocked(false),
	spread_increase(true), 	// Init spread
	y_min(PLAYER_Y), x_min(PLAYER_X), 				// (x,y) Cooridinates of top left corner
	action(Standing)								// Initial Action
{
	/* empty constructor */
}


// Animate stickman for the next frame
void Stickman::nextFrame() {
	if (is(Walking)&& !blocked) continue_walk();
}

// Instruct the stickman to start an action
void Stickman::start(Action instruction) {
	if (action == instruction) return;

	action = instruction;

//	if (action == Standing)
//		spread = (rand() % 2) + 5;
}

int nod = 0;
// Take a single step in either direction
void Stickman::continue_walk() {

	// If the spread is at the an extreme, change directions
	if (spread >= MAN_SPREAD || spread <= -MAN_SPREAD) {
		spread_increase = ! spread_increase;
		nod = (nod + 1) % 3;
	}
	
	// Depending on the direction of the spread, +/- a step length
	spread += spread_increase ? speed : -speed;
}

Color Stickman::color = Black;
// Paint the stickman onto the Game window
void Stickman::paint(GameWindow& GW) {

	GW.setColor(Black);
	// Draw Torso (pelvis -> centre of head)
	GW.drawLine(PLAYER_X,y_min + MAN_PELVIS,PLAYER_X, y_min + MAN_HEAD);
	// Draw Arms
	GW.drawLine(PLAYER_X,y_min + MAN_ARMS,PLAYER_X - spread, y_min + MAN_HANDS);
	GW.drawLine(PLAYER_X,y_min + MAN_ARMS,PLAYER_X + spread, y_min + MAN_HANDS);
    // Draw Head
    GW.drawCircle(PLAYER_X,y_min + MAN_HEAD-nod,MAN_HEAD_RAD,true);
	// Draw Leg
	GW.setColor(color);
	GW.drawLine(PLAYER_X,y_min + MAN_PELVIS,PLAYER_X - spread, y_min + MAN_FEET);
	GW.drawLine(PLAYER_X,y_min + MAN_PELVIS,PLAYER_X + spread, y_min + MAN_FEET);
}

int Stickman::min_x() {
	return x_min - PLAYER_WIDTH/2;
}

int Stickman::max_x() {
	return PLAYER_WIDTH/2 + x_min;
}

int Stickman::min_y() {
	return y_min - PLAYER_HEIGHT;
}

int Stickman::max_y() {
	return y_min;
}
