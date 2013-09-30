// Defining all Tunable constants:
#ifndef _SETTINGS_H
#define _SETTINGS_H

// Initial display dimensions and position
#define DISPLAY_WIDTH	800
#define DISPLAY_HEIGHT	600
#define DISPLAY_X		20
#define DISPLAY_Y		20
#define DISPLAY_BORDER  5
#define DISPLAY_NAME 	"Platformer"

// Angle of a circle
#define CIRCLE_ANGLE (360 * 64)

// Number of seconds to sleep when creating window
#define SLEEP_SECONDS 3

// Number and names of Colors
#define NUM_COLORS 13
#define COLOR_NAMES { "white", "black", "red", "green", "light blue", "orange", "yellow", "pink", "dark orange", "beige", "light grey", "dark grey", "blue" }
#define BACKGROUND_COLOR White

// Dimensions of a block
#define BLOCK_HEIGHT 40
#define BLOCK_WIDTH  40

// Height a block should be placed at
#define BLOCK_Y (DISPLAY_HEIGHT - BLOCK_HEIGHT)

// Maximum height a stack of blocks can be
#define MAX_BLOCK_STACK	7
//((DISPLAY_HEIGHT / BLOCK_HEIGHT) - 2)
#define MIN_BLOCK_STACK 3
#define MAX_DANGERS 3

#define PROBS { PROB_ZERO = 12, PROB_LAVA = 35, PROB_GUN = 45, PROB_UP = 65, PROB_DOWN = 80, PROB_STAY = 100, PROB_COIN = 20 }

// Diameter and initial position of the sun
#define SUN_OUTER_RADIUS 20
#define SUN_INNER_RADIUS (SUN_OUTER_RADIUS - 2)
#define SUN_X 200
#define SUN_Y 50

#define SCORE_Y 40
#define SCORE_X 10
#define HEALTH_X SCORE_X
#define HEALTH_Y (SCORE_Y + 20)
#define HP_BAR_X HEALTH_X
#define HP_BAR_Y (HEALTH_Y + 25)
#define HP_BAR_WIDTH 130
#define HP_BAR_HEIGHT 10

// Coin size and position
#define COIN_RADIUS 6
#define COIN_Y  (BLOCK_WIDTH /2)
#define COIN_X (BLOCK_WIDTH / 2)

#define FLAG_WIDTH 100
#define FLAG_HEIGHT 70
#define POLE_WIDTH 6
#define POLE_HEIGHT 400
#define BALL_RADIUS 10
// Formula for the active block that mario is on
// and the number of on-screen blocks
#define ACTIVE_BLOCKS (DISPLAY_WIDTH / BLOCK_WIDTH)
#define ACTIVE_BLOCK  4
#define WALL_BLOCKS 8
#define LEVEL_BLOCKS (ACTIVE_BLOCK + 2)
#define WORLD_LENGTH 200

// Dimensions of the player
#define PLAYER_HEIGHT 80
#define PLAYER_WIDTH  30
#define PLAYER_X_OFFSET (- BLOCK_WIDTH/2) 

// Where the player should appear on screen
#define PLAYER_X (((ACTIVE_BLOCK + 1) * BLOCK_WIDTH) + PLAYER_X_OFFSET) 
#define PLAYER_Y (DISPLAY_HEIGHT - 3*BLOCK_HEIGHT - PLAYER_HEIGHT) 
#define JUMP_HEIGHT (1.95 * BLOCK_HEIGHT)

// Initial offset of the level (so the player can move left)
#define INIT_X_OFFSET (WALL_BLOCKS * BLOCK_WIDTH)
#define INIT_Y_OFFSET (- BLOCK_HEIGHT * 3)
// Default values but adjustable via command-line
#define DEFAULT_RUN_SPEED	4
#define DEFAULT_JUMP_SPEED	5
#define DEFAULT_SUN_SPEED	2
#define DEFAULT_FPS 30

#define BULLET_SPEED 5
#define DAMAGE 2

// Convert from a distance to speed
#define SPEED(d,FPS) std::max(1,(DEFAULT_FPS*(d)/FPS))

// Number of micro-seconds in a second
#define MICRO 1000000

// Length of the resize message (pixels)
#define MSG_LENGTH 50

// Stickman lengths/distances
#define MAN_X (PLAYER_WIDTH / 2)	// X Center Position
#define MAN_PELVIS 50				// Pelvis joint height
#define MAN_ARMS 30					// Arms joint height
#define MAN_HEAD 20				// Head height
#define MAN_HEAD_RAD 8				// Radius of head
#define MAN_SPREAD 11				// Spread for legs and arms
#define MAN_HANDS MAN_PELVIS		// Height of hands
#define MAN_FEET PLAYER_HEIGHT		// Height of feet
#define MAN_SPEED 2					// Length of a single step

#define PAPER_SCALE (1/6.0)
#define PENCIL_NECK_HEIGHT (60)
#define PENCIL_LEAD_HEIGHT (PENCIL_NECK_HEIGHT/3)
#define PENCIL_EDGE (BLOCK_WIDTH/5)
#define PENCIL_RING 8

#endif
