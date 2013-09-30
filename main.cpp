#include <cstdlib>
#include "window.h"
#include "level.h"
#include "stickman.h"
#include <iostream>
#include "game.h"

int main (int argc, char* argv[]) {
	
	Game* G = createGame(argc,argv);
	if (G == NULL) return 1;

	// Event Loop
	while (! G->quit()) {
		G->handleEvents();
		G->update();
		G->sleep();
	}

	delete G;
	return 0;
}

