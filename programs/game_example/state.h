#pragma once

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

// Due to the game's simplicity, all information
// is directly visible to the user, without information hiding.

typedef struct ball {
	Vector2 position;
	int radius;
}* Ball;

typedef struct state {
	Rectangle character;				// character position and size
	struct ball ball1, ball2;			// object positions and sizes
	int score;							// current score
	bool playing;						// true while game is in progress
	bool game_over;						// true on the frame where game_over just happened (becomes false next frame)
}* State;


// Creates and returns the initial game state

State state_create();

// Updates game state after 1 frame.

void state_update(State state);