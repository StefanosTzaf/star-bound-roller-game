#include <stdlib.h>

#include "state.h"

// Initializes game state

static void state_init(State state) {
	state->score = 0;
	state->playing = true;

	state->character.width = 47;
	state->character.height = 39;
	state->character.x = 80;
	state->character.y = SCREEN_HEIGHT / 2 - state->character.height / 2;

	state->ball1.radius = 15;
	state->ball1.position.x = SCREEN_WIDTH / 3.;
	state->ball1.position.y = state->ball1.radius;

	state->ball2.radius = 30;
	state->ball2.position.x = 2 * SCREEN_WIDTH / 3.;
	state->ball2.position.y = state->ball2.radius;
}

// Creates and returns the initial game state

State state_create() {
	State state = malloc(sizeof(*state));
	state_init(state);
	return state;
}

// Updates game state for the next frame
//
// For simplicity, we directly use IsKeyDown(key)
// to check keys. A better implementation would take key state
// as an argument, so we can test this function with
// proper tests.
//
void state_update(State state) {
	state->game_over = false;		// true lasts only for 1 frame

	if (state->playing) {
		state->character.x += IsKeyDown(KEY_LEFT) ? 1 : 3;

		// wrap
		if (state->character.x > SCREEN_WIDTH) {
			state->character.x = 0;
			state->score++;
		}

		state->ball1.position.y = (int)(state->ball1.position.y + 5) % SCREEN_HEIGHT;
		state->ball2.position.y = (int)(SCREEN_HEIGHT + state->ball2.position.y - 7) % SCREEN_HEIGHT;

		if (IsKeyDown(KEY_UP))
			state->character.y -= 3;
		else
			state->character.y += 1;

		// collision checks
		if (state->character.y < 0 ||
			state->character.y > SCREEN_HEIGHT - state->character.height ||
			CheckCollisionCircleRec(state->ball1.position, state->ball1.radius, state->character) ||
			CheckCollisionCircleRec(state->ball2.position, state->ball2.radius, state->character)) {

			state->playing = false;
			state->game_over = true;
		}

	} else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
		state_init(state);		// reset to initial state
	}
}