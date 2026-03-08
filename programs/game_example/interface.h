#pragma once

#include "state.h"

// Initializes the game interface
void interface_init();

// Closes the game interface
void interface_close();

// Draws one frame using the current game state
void interface_draw_frame(State state);