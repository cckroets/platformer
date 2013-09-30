#include <vector>
#include "window.h"
#include "stickman.h"

#ifndef LEVEL_H
#define LEVEL_H
class HUD;

// A Tile (Space) in the level. 
class Tile : public Picture {

private:
	// Height of the tile. Zero means there is no tile. 
	int height;
	
	// Position of the tile relative to the screen (Not constant!)
	int x_pos;
	int y_pos;

public:
	// Default Tile
	Tile(); 
	
	// Construct a regular tile with a certain height 
	Tile(int height);

	// Some tiles may have an effect when landing on them, this one does not
	virtual void effect(HUD& d);// { };

	// Corners of the bounding box
	int min_x() const;
	int min_y() const;
	int max_x() const;
	int max_y() const;

	// Change the position of the tile
	void setPos(int x, int y) {
		x_pos = x;
		y_pos = y;
	}

	// Paint a tile a certain way
	virtual void paint(GameWindow& GW);

	// Return the height of the tile
	int getHeight(); 
};

// A special tile that has a coin on it
class CoinTile: public Tile {

private:
	bool coin_taken;

public:
	
	CoinTile() : coin_taken(false) { };
	CoinTile(int height) : Tile(height), coin_taken(false) { };
	void effect(HUD&);
	void paint(GameWindow& GW);
};

// A special tile that is made of lava
class LavaTile : public Tile {
protected:
	Color color;

public:
	static int damage;
	LavaTile() { };
	LavaTile(int);
	void effect(HUD&);
	void paint(GameWindow& GW);
};

class FlagTile : public Tile {

public:
	FlagTile() { };
	FlagTile(int h) : Tile(h) { };
	void effect(HUD&);
	void paint(GameWindow& GW);
};

class GunTile : public LavaTile {

private:
	int bullet_y;

public:
	static int speed; 
	GunTile() { };
	GunTile(int h) : LavaTile(h), bullet_y(0) { color=Black; };
	void effect(HUD&);
	void paint(GameWindow& GW);
};

class HUD : public Picture {

private:
	bool finished;
	int hp;
	int coins;

public:
	void addCoin() { coins++; };
	void takeDamage(int dmg) { hp -= std::min(hp,dmg); };
	void finish()  { finished = true; };
	bool died();
	bool won()	{ return finished; };

    void paint(GameWindow& GW);
	HUD() : hp(100), coins(0), finished(false) { };
};

/* A randomly generated level given a length and max height a tile can be.
Each tile to the right  */
class Level : public Picture {

private:
	// Vector of spaces, representing the level from left to right.
	// Each uint represents the height of that space. Spaces increase
	// in height by at most one, but may decrease to zero. 
	std::vector<Tile*> spaces;
	HUD hud;

	// Moving speeds
	int run_speed;
	int jump_speed;
	int jump_peak;
	
	// True if jumping or blocked
	bool jumping;
	bool block;

	void start_jump();

public:
	static long y_offset;
	static long x_offset;

	// Empty construction does nothing;
	Level() { };
	~Level();
	// Create a random level of a specified length and height
	Level(int length, int rs, int js); 

	// Return the index of the first and last blocks on the screen
	// (The ones that need to be redrawn)
	uint first_block();
	uint last_block();

	// Returns the index of the first active block,
	// the next one may also be active simultaneously
	uint active_block();

	// Returns a const ref to the ith tile  
	const Tile& getTile(size_t i);  
	
	// Paint the world onto the screen
	void paint(GameWindow& GW);
	
	bool damaging();
	bool gameOver();
	void jump(bool high);
	void nextFrame();
	void move(Key);
	bool floating();
	bool blocked();
};

// The always-moving sun. 
class Sun : public Picture {

private:
	int x_pos;		// X Position
	int speed;

public:
	Sun() { };

	// Create an instance of a sun with a specified speed
	Sun(int s);
	
	// Move the sun for the next frame
	void nextFrame();

	// Draw the sun
	void paint(GameWindow& GW);
};


#endif
