
# Star Bound Roller Game

This repository contains a course project for **Data Structures and Programming Techniques**.  
The goal of the assignment was to implement a small platform game in C, with a graphical interface and
interactive gameplay, while also applying data structure design, information hiding, testing, and performance-oriented implementation choices.

The original assignment description was provided in Greek here: https://k08.chatzi.org/2023/projects/project1/

## What Was Provided and What Was Implemented

To make the boundaries of the work clear:

### Provided as part of the assignment

- The general project skeleton and repository structure
- The public interface in `include/state.h`
- The raylib graphics library and related build files
- The ADT library headers and implementations used by the course infrastructure
- The existing abstract data type modules such as:
  - `ADTList`
  - `ADTMap`
  - `ADTPriorityQueue`
  - `ADTQueue`
  - `ADTSet`
  - `ADTStack`
  - `ADTVector`
- Support files such as `acutest.h`, common type definitions, and makefiles
- The `programs/game_example` sample project used as a reference
- The assignment requirements, which specified the expected modules and behavior

### Implemented in this project

- The game-state logic in `modules/state.c`
- The alternative optimized implementation in `modules/state_alt.c`
- The utility functions for the set-based solution in `modules/set_utils/`
- The interface logic for the game in `programs/game/`
- The tests for the required functionality in `tests/` and `modules/set_utils/`
- Gameplay adjustments and small design choices beyond the minimum assignment requirements

In short, the ADT infrastructure, raylib integration, and assignment scaffolding were provided, while the game logic, 
alternative implementation, tests, and final game behavior were my own work.

## Project Overview

The game is a side-scrolling platform game where a ball keeps moving forward across moving platforms. Some platforms are 
unstable and start falling when the ball lands on them. Stars are placed between platforms and increase the score when collected.

The objective is to survive as long as possible and collect as many points as possible without falling.

## Implementations

Two implementations of the game state are included.

### `modules/state.c`

This is the straightforward implementation based on a `Vector` of objects. It is simpler and easier to reason about, and it 
follows the assignment requirements directly.

### `modules/state_alt.c`

This is the more efficient implementation and the default one used by the project. It is based on a `Set`, so 
range queries and nearby-object processing can be handled more efficiently.

The main advantage of the alternative implementation is that it avoids scanning the entire world for operations such as:

- retrieving visible objects
- updating nearby objects only
- limiting collision checks to relevant candidates

## Switching Between Implementations

If you want to use the first implementation (`state.c`) instead of the default optimized one (`state_alt.c`),
you need to edit the makefiles of the game and the tests:

- in the makefile for `programs/game`
- in the makefile for `tests`

Comment out the `OBJS` line that contains `state_alt.c` and uncomment the one that contains `state.c`.

The `state_alt.c` implementation is the default one because it is more efficient, but both versions are included for completeness.

## Gameplay Notes

Some details of the gameplay were intentionally adjusted within the flexible bounds allowed by the assignment:

- Pressing the left arrow pushes the ball slightly backward, which makes the movement feel more natural.
- Unstable platforms are generated with a probability of 20%.
- The interface follows the ball horizontally by drawing visible objects with an x-offset relative to the ball position.

## Design Notes

### State update logic in `state.c`

In `state_update`, the horizontal and vertical movement of the ball are handled before iterating through the objects.

To avoid adding extra loops and increasing complexity, the same loop that traverses the object vector is also used to:

1. update the vertical movement of platforms
2. keep the ball aligned with a platform while the ball is idle
3. handle collisions with platforms, stars, and other game situations such as falling

All of this logic is executed only when the game is active, or when the game is paused and the `N` key is used to advance
the simulation by one frame for debugging.

The logic for restart, pause, and other state transitions is kept outside the main gameplay section 
so that the next call to `state_update` sees a fully updated game state.

### Set-based utilities and `state_alt.c`

The functions required for the set-based solution are implemented in `modules/set_utils/`, together with tests
for those utilities and the object comparison logic.

The second implementation follows a similar gameplay logic to the first one, but uses a `Set` so that objects
in a specific x-range can be retrieved efficiently thanks to the ordering of the structure.

### Interface

In the interface module, objects are drawn using an offset derived from the ball's x-coordinate. This makes it 
easier to map world coordinates to actual screen coordinates and render only the visible part of the game world.

## Build and Run

The project uses the provided makefiles.

Typical commands:

```bash
make
make run
make clean
```

You can also run the game and tests through the VS Code tasks included in the workspace.

## Windows / WSL Notes

For Windows users working through WSL, the assignment instructions require `VcXsrv` in order to run the graphical application.

Recommended setup from the assignment:

```bash
curl https://k08.chatzi.org/vscode/config.sh | bash
```

Then:

1. Install `VcXsrv`
2. Launch it with settings:
   - Multiple windows
   - Clipboard (Primary Selection)
   - Start no client
   - Disable access control
3. Keep it running while working on the project

If running manually from the console in WSL, you may need:

```bash
export DISPLAY=$(ip route list default | awk '{print $3}'):0
export LIBGL_ALWAYS_INDIRECT=1
```

The provided makefiles already handle this automatically in the relevant cases.

## Repository Structure

```text
include/           Public headers and provided interfaces
lib/               raylib support files
modules/           Core game-state implementations and utilities
programs/game/     Main playable game
programs/game_example/  Example project provided with the assignment
tests/             Unit tests
```

## Summary

This project was developed as part of the **Data Structures and Programming Techniques** course and focuses on:

- modular design and information hiding
- game-state management in C
- testing of core logic
- comparing a straightforward and an optimized implementation
- integrating data structures into a playable graphical application
