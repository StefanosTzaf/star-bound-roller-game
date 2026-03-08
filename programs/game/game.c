//////////////////////////////////////////////////////////////////////////////
//
// Example of creating a game using the raylib library
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"
#include <stdlib.h>

State state;

void update_and_draw() {
	// state_update has no dependency on raylib.h (so it can be replaced in the future
	// without changing state_update). update_and_draw, which calls state_update,
	// passes the key state to it.

	struct key_state keys1;
	keys1.down = IsKeyDown(KEY_DOWN);
	keys1.up = IsKeyDown(KEY_UP);
	keys1.enter = IsKeyDown(KEY_ENTER);
	keys1.left = IsKeyDown(KEY_LEFT);
	keys1.right = IsKeyDown(KEY_RIGHT);
	keys1.n = IsKeyDown(KEY_N);
	keys1.p = IsKeyPressed(KEY_P);

	state_update(state,&keys1);
	interface_draw_frame(state);
}

int main() {
	state = state_create();
	interface_init();

	// This call continuously invokes update_and_draw until the user closes the window
	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}



