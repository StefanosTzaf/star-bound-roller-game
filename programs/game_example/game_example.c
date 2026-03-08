//////////////////////////////////////////////////////////////////////////////
//
// Example of creating a game using the raylib library
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;

void update_and_draw() {
	state_update(state);
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



