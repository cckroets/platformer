#include "game.h"
#include <cstdlib>
#include <iostream>
#include <cassert>


bool valid_fps(int fps) {
	return (fps > 0) && (fps < 100);
}

bool valid_option(int speed) {
    return (speed >= -5) && (speed <= 5);
}

Game* createGame(int argc, char* argv[]) {

	int fps_opt   = DEFAULT_FPS;
	int sun_opt   = 0;
	int run_opt   = 0;
	int jump_opt  = 0;

	if (argc == 5) {
		fps_opt = atoi(argv[4]);
		assert(valid_fps(fps_opt));
	
		sun_opt = atoi(argv[3]) - 5;
		assert(valid_option(sun_opt));

		run_opt = atoi(argv[1]) - 5;
		assert(valid_option(run_opt));
	
		jump_opt = atoi(argv[2]) - 5;
		assert(valid_option(jump_opt));
	} else if (argc != 1) {
 
		std::cout << "Provide 0 or 4 options: The first 3 options should be for Run, Jump, and Sun speeds, and are numbers in the range 0-10.";
		std::cout << "\nSetting each speed equal to 5 and a framerate of 30 is suggested for normal play.";
		std::cout << "\nThe fourth option is the FPS which can be anywhere in the range of [1-100]" << std::endl;
		return NULL;
	}
	
	int fps = fps_opt;
	int sun_speed = SPEED(DEFAULT_SUN_SPEED+sun_opt,fps);
	int run_speed = SPEED(DEFAULT_RUN_SPEED+run_opt,fps);
	int jump_speed = SPEED(DEFAULT_JUMP_SPEED+jump_opt,fps);
	
	return new Game(fps,run_speed,jump_speed,sun_speed);
}


Game::Game(int fps, int rs, int js, int ss): paused(0), has_quit(0), fps(fps), Player(rs), TheSun(ss), World(WORLD_LENGTH,rs,js) {

	LavaTile::damage = SPEED(DAMAGE,fps);
	GunTile::speed = SPEED(BULLET_SPEED,fps); 

	// Push game objects onto drawables list
	Drawables.push_back(&BGround);
	Drawables.push_back(&TheSun);
	Drawables.push_back(&World);
	Drawables.push_back(&Player);
}

// Handle key events
void Game::handleEvents() {

	Key used;
	bool standing;
	int input = GW.getKeyEvent(used);
	if (! input) return;

	switch (used) {
		case Quit: 
			has_quit = true;
			break;
		case Pause:
			if (input == KeyPress) paused = ! paused;
			break;
		case Up:
			standing = Player.is(Standing) || World.blocked();
			if (input == KeyPress) World.jump(! standing); 
			break;
		default: // Left or Right
			if (used != Left && used != Right) break; 
			facing = used;
			if (Player.is(Standing) && input == KeyPress) 
				Player.start(Walking);
			else 
				Player.start(Standing);
			break;
	}
}

// Quit the game!
bool Game::quit() {
	bool gameover = has_quit || World.gameOver();
	if (gameover) {
		usleep(2000000);
		GW.close();
	}
	return gameover;
};

void Game::sleep() {
	usleep(MICRO/fps);
}

void Game::update() {

	// If the game is paused, don't update or paint
	if (paused) return;

	// Make the player move
	if (Player.is(Walking)) World.move(facing);

	// Let the player know if he is blocked or floating
	// so that he does not walk
	if (World.floating() || World.blocked()) 
		Player.block();
	else
		Player.unblock();	

	// Update each picture, then draw it
	for (int p = 0; p < Drawables.size(); p++) {
		Drawables[p]->nextFrame();
		Drawables[p]->paint(GW);
	}

	GW.repaint();
}
