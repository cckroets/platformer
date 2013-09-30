#include "level.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <iostream>

int real_min_x();
int real_max_x();
int player_min_x();
int player_min_y();
int player_max_x();
int player_max_y();
bool overlap(int,int,int,int);

/********************** TILE *****************/

Tile::Tile(int height) : height(height) {  };

Tile::Tile() : height(MIN_BLOCK_STACK) { /*Blank*/ }

int Tile::getHeight() {
	return height;
}

void Tile::effect(HUD& hud) {
	bool ox = overlap(real_min_x(),real_max_x(),min_x(),max_x());
	bool oy = player_max_y() == min_y();
	if (ox & oy)
		Stickman::color = Black;
}

/********************** Coin *******************/

bool touches_circle(int cent_x, int cent_y, int rad) {

	bool touches_y = overlap(player_min_y(),player_max_y(), cent_y - rad, cent_y + rad);
	bool touches_x = overlap(player_min_x(),player_max_x(), cent_x - rad, cent_x + rad);

	return touches_y && touches_x; 
}

void CoinTile::effect(HUD& hud) {
	Tile::effect(hud);
	int coin_x = min_x() + COIN_X;
	int coin_y = min_y() - COIN_Y;

	if (! coin_taken && touches_circle(coin_x,coin_y,COIN_RADIUS)) {
		hud.addCoin();
		coin_taken = true;
	}
}

void CoinTile::paint(GameWindow& GW) {
	
	// Paint the ordinary tile
	Tile::paint(GW);
	
	if (! coin_taken) {
		// Paint the coin on top of the tile
		GW.setColor(Black);
		int centre_x = min_x() + COIN_X;
		int centre_y = min_y() - COIN_Y;
		GW.drawCircle(centre_x,centre_y, COIN_RADIUS, false);
		GW.setColor(Yellow);
		GW.drawCircle(centre_x,centre_y, COIN_RADIUS, true);
	}
}

/********************* Lava ******************/

LavaTile::LavaTile(int h) : Tile(h) {
	int rnd = rand() % 2;
	if (rnd) color = DarkBlue;
	else color = Red;
};


bool overlap(int k1, int k2, int j1, int j2) {

	return std::max(k1,j1) <= std::min(k2,j2);
}


int LavaTile::damage = 0;

bool touches_lava(int x_min, int x_max, int y_min, int y_max) {
	bool touching = player_max_y() == y_min;
	bool overlap_x = overlap(x_min, x_max, player_min_x(), player_max_x());

	return overlap_x & touching;
}

void LavaTile::effect(HUD& hud) {
	if (touches_lava(min_x()+BLOCK_WIDTH/2,max_x()-BLOCK_WIDTH/2, min_y(), max_y())) {
		Stickman::color = this->color;
		hud.takeDamage(damage);
	}
}

void LavaTile::paint(GameWindow& GW) {

	int neck = PENCIL_NECK_HEIGHT;
    int lead_h = PENCIL_LEAD_HEIGHT;
	int lead_w = BLOCK_WIDTH/3;

    // Draw Shaft
    GW.setColor(color);
    GW.drawRect(min_x(),min_y()+neck,BLOCK_WIDTH,DISPLAY_HEIGHT-min_y(),true);

    // Draw Tip and Outline
    GW.setColor(Brown);
    GW.drawTrig(min_x(),min_y(),BLOCK_WIDTH,neck,true);
    GW.setColor(Black);
    GW.drawTrig(min_x(),min_y(),BLOCK_WIDTH,neck,false);
    GW.drawRect(min_x(),min_y()+neck,BLOCK_WIDTH,DISPLAY_HEIGHT-min_y());

    //Draw Lead
	GW.setColor(color);
    GW.drawTrig(min_x()+BLOCK_WIDTH/2-lead_w/2,min_y(),lead_w,lead_h,true);
	GW.setColor(Black);
	GW.drawTrig(min_x()+BLOCK_WIDTH/2-lead_w/2,min_y(),lead_w,lead_h,false);

}

/********************** GUN *******************/

int GunTile::speed = 3;

void GunTile::effect(HUD& hud) {
	LavaTile::effect(hud);
	if (touches_circle(min_x() + COIN_X, bullet_y-Level::y_offset, COIN_RADIUS)) {
		hud.takeDamage(10);
		bullet_y = min_y() + COIN_Y - Level::y_offset;
	}
}

void GunTile::paint(GameWindow& GW) {

	int neck = PENCIL_NECK_HEIGHT;
	int lead = PENCIL_LEAD_HEIGHT;

	// Draw Shaft
    GW.setColor(Orange);
    GW.drawRect(min_x(),min_y()+neck,BLOCK_WIDTH,DISPLAY_HEIGHT-min_y(),true);

	// Draw Edges
    GW.setColor(DarkOrange);
    GW.drawRect(min_x()+BLOCK_WIDTH/3,min_y()+neck,BLOCK_WIDTH/3,DISPLAY_HEIGHT,true);

	// Draw Tip and Outline
    GW.setColor(Brown);
    GW.drawTrig(min_x(),min_y(),BLOCK_WIDTH,neck,true);
    GW.setColor(Black);
	GW.drawTrig(min_x(),min_y(),BLOCK_WIDTH,neck,false);
	GW.drawRect(min_x(),min_y()+neck,BLOCK_WIDTH,DISPLAY_HEIGHT-min_y());

	//Draw Lead
	GW.drawTrig(min_x()+BLOCK_WIDTH/2-BLOCK_WIDTH/6,min_y(),BLOCK_WIDTH/3,lead,true);

	// Draw Bullet
	int bheight = bullet_y - Level::y_offset;
	if (bheight < min_y()){
		GW.drawCircle(min_x()+COIN_X,bheight,COIN_RADIUS,false);
		GW.setColor(Red);
		GW.drawCircle(min_x()+COIN_X,bheight,COIN_RADIUS,true);
	} 
	if (bheight < 0) 
		bullet_y = min_y() + COIN_Y -Level::y_offset;
	else
		bullet_y -= speed;
}

/********************** FLAG ******************/

bool touches_flag(int x) {
	return x < player_max_x();
}

void FlagTile::effect(HUD& hud) {
	if (touches_flag(min_x()+BLOCK_WIDTH/2)) 
		hud.finish();
}

void FlagTile::paint(GameWindow& GW) {
	Tile::paint(GW);
	
	GW.setColor(Black);
	int x = min_x() + BLOCK_WIDTH/2;
	int y = min_y() - POLE_HEIGHT;
	GW.drawRect(x,y,POLE_WIDTH,POLE_HEIGHT,true);
	GW.drawRect(x+POLE_WIDTH,y,FLAG_WIDTH,FLAG_HEIGHT,false);
	GW.setColor(White);
	GW.drawRect(x+POLE_WIDTH,y,FLAG_WIDTH,FLAG_HEIGHT,true);
	GW.setColor(Black);
	GW.drawCircle(x,y,BALL_RADIUS,true);
	GW.drawText(x+22,y+45,"Finish");
}

/********************** Level *****************/


struct TileGenerator {

private:
	int prevHeight;	  // Height of previous brick
	int prevDangers;  // Number of 0-heights/lava immediately past

	// Percentage probabilities
	enum TileProbs PROBS;

	Tile* randCoin(int height) {
		int type = rand() % 100;
		if (type < PROB_COIN)
			return new CoinTile(height);
		else
			return new Tile(height);
	}

	Tile* getNewTile() {

		Tile* newTile = NULL;
		int type = rand() % 100;
		
		if (type < PROB_ZERO && prevDangers < MAX_DANGERS) {
			prevDangers++; 
			return new Tile(0);
		} else if (type < PROB_LAVA && prevDangers < MAX_DANGERS) {
			prevDangers++;
			return new LavaTile(prevHeight);
		} else if (type < PROB_GUN && prevDangers < MAX_DANGERS) {
			prevDangers++;
			return new GunTile(prevHeight);
		} else if ((type < PROB_UP) && (prevDangers < MAX_DANGERS)) {
			prevHeight = std::min(prevHeight + 1, MAX_BLOCK_STACK);
		} else if (type < PROB_DOWN) {
			prevHeight = std::max(prevHeight - rand() % 3, MIN_BLOCK_STACK);
		}
		prevDangers = 0;
		return randCoin(prevHeight);
	}
	

public:
	TileGenerator() : prevHeight(MIN_BLOCK_STACK), prevDangers(MAX_DANGERS) { 
		srand(time(NULL)); // Random seed
	};

	// Generate random tile
	Tile* operator()() {
		return getNewTile();
	}
};

long Level::x_offset = INIT_X_OFFSET;
long Level::y_offset = INIT_Y_OFFSET;

// Length of the level, run speed, jump speed
Level::Level(int length, int rs, int js): 
	spaces(length),
	jump_peak(0), jumping(false), block(false),
	run_speed(rs), jump_speed(js) {

	// A Random Tile Generator
	TileGenerator Gen;
	
	// Generate random tiles
	std::generate(spaces.begin(),spaces.end(),Gen);

	// Add a wall to stop player from moving left	
	for (int i = 0; i < LEVEL_BLOCKS; i++)
  		spaces.insert(spaces.begin(), new Tile(3));

	for (int i = 0; i < WALL_BLOCKS; i++)
    	spaces.insert(spaces.begin(), new Tile(6));

	spaces.insert(spaces.end(), new FlagTile(MIN_BLOCK_STACK));

	for (int i = 0; i < WALL_BLOCKS*3; i++)
		spaces.insert(spaces.end(), new Tile(i+MIN_BLOCK_STACK));
}

Level::~Level() {
	for (int i = 0; i < spaces.size(); i++)
		delete spaces[i];
}

uint Level::first_block() {
	return x_offset / BLOCK_WIDTH;
}

uint Level::last_block() {
	return (x_offset + DISPLAY_WIDTH) / BLOCK_WIDTH;
}

uint Level::active_block() {
	return first_block() + ACTIVE_BLOCK;
}

const Tile& Level::getTile(size_t i) {
	return *spaces[i];
}

void Tile::paint(GameWindow& GW) {

	// Eraser Height
	int eh = BLOCK_HEIGHT/2;

	GW.setColor(Orange);
	GW.drawRect(x_pos,y_pos+eh,BLOCK_WIDTH,DISPLAY_HEIGHT-y_pos,true);
	GW.setColor(DarkOrange);
	int fifth = BLOCK_WIDTH/5;
	
	// Draw edges
    GW.drawRect(min_x()+BLOCK_WIDTH/5,min_y(),BLOCK_WIDTH*3/5,DISPLAY_HEIGHT-y_pos,true);

	// Draw Eraser
	GW.setColor(Magenta);
	GW.drawOval(x_pos,y_pos-eh/2,BLOCK_WIDTH,eh,true);
	GW.setColor(Black);
	GW.drawOval(x_pos,y_pos-eh/2,BLOCK_WIDTH,eh,false);
	GW.setColor(Magenta);
	GW.drawRect(x_pos,y_pos,BLOCK_WIDTH,eh,true);
	GW.setColor(Black);
	GW.drawLine(x_pos,y_pos,x_pos,y_pos+eh);
	GW.drawLine(max_x(),y_pos,max_x(),y_pos+eh);

	// Draw silver part
	int h = PENCIL_NECK_HEIGHT*3/4;
    GW.drawRect(x_pos,y_pos+eh,BLOCK_WIDTH,h,false);
	GW.setColor(Grey);
	GW.drawRect(x_pos,y_pos+eh,BLOCK_WIDTH,h,true);
	GW.setColor(DarkGrey);
	int y = y_pos + eh + PENCIL_RING;
	for (int i = 0; i < 3; i++, y+=PENCIL_RING/2) {
		GW.drawLine(x_pos,y,x_pos+BLOCK_WIDTH,y);
        GW.drawLine(x_pos,y+3*PENCIL_RING,x_pos+BLOCK_WIDTH,y+3*PENCIL_RING);
	}
	
	// Draw outline
    GW.setColor(Black);
    GW.drawRect(x_pos,y_pos+eh,BLOCK_WIDTH,DISPLAY_HEIGHT-y_pos,false);
}

int Tile::min_x() const {
	return x_pos;
}

int Tile::min_y() const {
	return y_pos;
}

int Tile::max_x() const {
	return x_pos + BLOCK_WIDTH;
}

int Tile::max_y()  const {
	return y_pos + height*BLOCK_HEIGHT;
}

inline int player_min_x() {
	return PLAYER_X - PLAYER_WIDTH/2;
}

inline int player_max_x() { 
	return PLAYER_X + PLAYER_WIDTH/2;
}

inline int real_max_x() {
	return PLAYER_X + Stickman::spread;
}

inline int real_min_x() {
	return PLAYER_X - Stickman::spread;
}

inline int player_min_y() {
	return PLAYER_Y;
}

inline int player_max_y() {
	return PLAYER_Y + PLAYER_HEIGHT;
}

bool on_tile(const Tile&);

bool touching(const Tile& tile) {
	bool over_tile = on_tile(tile);
	bool standing_on = tile.min_y() == player_max_y() && over_tile;
	bool left_t = tile.min_x() == player_max_x() && (player_max_y() > tile.min_y());
	bool right_t = tile.max_x() == player_min_x() && (player_max_y() > tile.min_y());
	return standing_on | left_t | right_t;
}


bool on_tile(const Tile& tile) {
	int l = tile.min_x();
	int r = tile.max_x();
	return (l <= player_min_x()) && (player_max_x() <= r);
}


// Return the max distance the player can fall due to floating
int float_dist(const Tile& t1, const Tile t2) {

	int max_fall = 0;

	if ((player_max_y() < t1.min_y()) && (player_max_y() < t2.min_y())) {
		int highest = std::min(t1.min_y(),t2.min_y());	
		max_fall = highest - player_max_y();
	}
	else if (on_tile(t1) && (t1.min_y() > player_max_y())) {
		max_fall = t1.min_y() - player_max_y();
	}
	else if (on_tile(t2) && (t2.min_y() > player_max_y())) {
		max_fall = t2.min_y() - player_max_y();
	}

	return max_fall;	
}

bool Level::floating() {
	return float_dist(getTile(active_block()),getTile(active_block()+1));
}

void Level::jump(bool high) {

    int ab = active_block();
    if (! jump_peak && float_dist(getTile(ab),getTile(ab+1)) == 0)
        if (high)
            jump_peak = JUMP_HEIGHT;
        else
            jump_peak = JUMP_HEIGHT/1.5;
}


// Moves the player if possible in direction dir. returns true iff successful
void Level::move(Key dir) {

	// Tiles of interest
	uint ab = active_block();
	Tile tile_left = getTile(ab);
	Tile tile_right = getTile(ab + 1); 

	// True iff the player is blocked by the bricks
	bool blocked_left  = tile_left.min_y() < player_max_y();
	bool blocked_right = tile_right.min_y() < player_max_y();
	int dist = 0;	

	switch (dir) {
		case Right:
			dist = blocked_right ? std::min(run_speed, tile_right.min_x() - player_max_x()) : run_speed;
			block = (dist == 0); 
			x_offset += dist;
			break;
		case Left:
			dist = blocked_left ? std::min(run_speed, player_min_x() - tile_right.min_x()) : run_speed;
			block = (dist == 0);
			x_offset -= dist; 
			break;
	}
}

bool Level::blocked() {
	return block;
}

bool Level::gameOver() {

	return hud.died() || hud.won();
}

void Level::nextFrame() {
	
    // Tiles of interest
    uint ab = active_block();
    Tile tile_left = getTile(ab);
    Tile tile_right = getTile(ab + 1);
	int dist; 
	if (jump_peak) {
		dist = std::min(jump_peak,jump_speed);
		jump_peak -= dist;
		y_offset -= dist;
	} else {
		dist = std::min(jump_speed,float_dist(tile_left,tile_right));
		y_offset += dist;
	}


	spaces[ab]->effect(hud);
	spaces[ab+1]->effect(hud);
}

// Paint only the necessary tiles to the screen
void Level::paint(GameWindow& GW) {

	uint first = first_block();
	uint last = last_block();
	int block, h;	

	// Draw each Tower
	for (block = first; block <= last; block++) {
		
		Tile* cur_tile = spaces[block];
		int height = cur_tile->getHeight();
		int x_pos = BLOCK_WIDTH * (block-first) - x_offset % BLOCK_WIDTH;

		if (height) {
			int y_pos = BLOCK_Y - height * BLOCK_HEIGHT - y_offset;
			cur_tile->setPos(x_pos,y_pos);
			cur_tile->paint(GW);
		} else 
			cur_tile->setPos(x_pos,DISPLAY_HEIGHT);
	}
	hud.paint(GW);
}

// Paint the head up display onto the screen
void HUD::paint(GameWindow& GW) {
	GW.setColor(White,true);
	GW.setColor(Black,false);

	std::ostringstream coin_msg, health_msg; 
	coin_msg << " Coins: " << coins;
	health_msg  << "Health: " << hp;

	float pct = hp / 100.0;
	GW.drawText(SCORE_X,SCORE_Y,coin_msg.str());
	GW.drawText(HEALTH_X,HEALTH_Y+12,health_msg.str());
    GW.setColor(Black);
    GW.drawRect(HP_BAR_X,HP_BAR_Y,HP_BAR_WIDTH,HP_BAR_HEIGHT,false);
	GW.setColor(Red);
	GW.drawRect(HP_BAR_X,HP_BAR_Y,HP_BAR_WIDTH,HP_BAR_HEIGHT,true);
	GW.setColor(Green);
    GW.drawRect(HP_BAR_X,HP_BAR_Y,HP_BAR_WIDTH*pct,HP_BAR_HEIGHT,true);

	if (died()) {
		GW.setColor(Red);
		GW.drawText(380,90,"YOU LOSE.");
	} else if (won()) {
		GW.setColor(Green);
		GW.drawText(380,90,"YOU WIN!");
	}

}


Sun::Sun(int s) : speed(s), x_pos(SUN_X) { }

// Move the sun for the next frame
void Sun::nextFrame() {
	x_pos += speed;
}

void Sun::paint(GameWindow& GW) {

	GW.setColor(Yellow);
	GW.drawCircle(x_pos, SUN_Y, SUN_OUTER_RADIUS, true);
	GW.setColor(Red);
	GW.drawCircle(x_pos, SUN_Y, SUN_OUTER_RADIUS, false);
	GW.setColor(Orange);
	GW.drawCircle(x_pos, SUN_Y, SUN_INNER_RADIUS, false);
}

bool HUD::died() {
	return (hp <= 0) || Level::y_offset >= 0;
}

