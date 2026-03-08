#pragma once

#include "raylib.h"
#include "ADTList.h"

#define PLATFORM_NUM 20		// how many platforms are generated on the track
#define SCREEN_WIDTH 900	// Screen width
#define SCREEN_HEIGHT 450	// Screen height

typedef enum {
	BALL, PLATFORM, STAR
} ObjectType;

typedef enum {
	IDLE, JUMPING, FALLING, MOVING_UP, MOVING_DOWN
} VerticalMovement;

// Information for each object
typedef struct object {
	ObjectType type;			// Type (Ball, Platform, Star)
	Rectangle rect;				// Object position and size. Rectangle is defined in include/raylib.h, line 213
	VerticalMovement vert_mov;	// Vertical movement type
	float vert_speed;			// Vertical speed
	bool unstable;				// (platforms only) true if unstable
}* Object;

// General information about game state
typedef struct state_info {
	Object ball;					// Ball information
	bool playing;					// true while game is active (false after game over)
	bool paused;					// true when game is paused
	int score;						// current score
}* StateInfo;

// Information about which keys are pressed
typedef struct key_state {
	bool up;						// true if the corresponding key is pressed
	bool down;
	bool left;
	bool right;
	bool enter;
	bool n;
	bool p;
}* KeyState;

// Game state (handle)
typedef struct state* State;


// Creates and returns the initial game state

State state_create();

// Returns the basic game information in state

StateInfo state_info(State state);

// Returns a list of all game objects in state
// whose x coordinate lies between x_from and x_to.

List state_objects(State state, float x_from, float x_to);

// Updates game state after 1 frame.
// keys contains the keys that were pressed during this frame.

void state_update(State state, KeyState keys);

// Destroys state and frees allocated memory.

void state_destroy(State state);