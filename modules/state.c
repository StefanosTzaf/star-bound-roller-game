
#include <stdlib.h>
#include <stdio.h>

#include "ADTList.h"
#include "ADTVector.h"
#include "state.h"

// Complete information for the game state.
// Type State is a pointer to this struct, but the struct itself
// is not visible to the user.

// Accepts a vector and a position, and swaps that position's contents with the last vector element
void vector_swap(Vector vec, int position) {
	Object temp = vector_get_at(vec, position);
	// Put the last vector element at position
	vector_set_at(vec, position, vector_node_value(vec, vector_last(vec)));
	// Put the original position content at the end of the vector
	vector_set_at(vec, vector_size(vec) - 1, temp);
}

struct state {
	Vector objects;			// Contains Object elements (platforms, stars)
	struct state_info info;	// General game-state information
	float speed_factor;		// Speed multiplier (1 = normal speed, 2 = double, etc)
};

static const float BALL_GROUND_SPEED = 4.0f;
static const float BALL_AIR_SPEED = 5.4f;
static const float BALL_RIGHT_SPEED_BONUS = 2.0f;
static const float BALL_LEFT_SPEED = 0.25f;
static const float BALL_JUMP_START_SPEED = 17.0f;
static const float BALL_JUMP_DECAY = 0.80f;
static const float BALL_FALL_ACCELERATION = 1.15f;
static const float BALL_MAX_FALL_SPEED = 8.0f;
static const float PLATFORM_SNAP_TOLERANCE = 6.0f;

static bool ball_is_over_platform(Object ball, Object platform) {
	float ball_left = ball->rect.x + 5;
	float ball_right = ball->rect.x + ball->rect.width - 5;
	float platform_left = platform->rect.x;
	float platform_right = platform->rect.x + platform->rect.width;

	return ball_right >= platform_left && ball_left <= platform_right;
}

static bool ball_can_snap_to_platform(Object ball, Object platform) {
	float ball_bottom = ball->rect.y + ball->rect.height;
	float platform_top = platform->rect.y;

	return ball_is_over_platform(ball, platform)
		&& ball_bottom >= platform_top - PLATFORM_SNAP_TOLERANCE
		&& ball_bottom <= platform_top + PLATFORM_SNAP_TOLERANCE;
}

static void destroy_object(Pointer value) {
	free(value);
}

static void state_reset(State state) {
	State new_state = state_create();
	if (new_state == NULL) {
		return;
	}

	Vector old_objects = state->objects;
	Object old_ball = state->info.ball;

	state->objects = new_state->objects;
	state->info = new_state->info;
	state->speed_factor = new_state->speed_factor;

	free(new_state);
	vector_set_destroy_value(old_objects, destroy_object);
	vector_destroy(old_objects);
	destroy_object(old_ball);
}

// Creates and returns an object

static Object create_object(
	ObjectType type,
	float x,
	float y,
	float width,
	float height,
	VerticalMovement vert_mov,
	float speed,
	bool unstable
) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	obj->vert_mov = vert_mov;
	obj->vert_speed = speed;
	obj->unstable = unstable;
	return obj;
}

// Adds objects to the track (which may already contain objects).
// Objects start from x = start_x and extend to the right.
//
// WARNING: all objects have x,y coordinates in a
// Cartesian plane.
// - On the x axis, 0 is the start of the track and coordinates
//   increase to the right.
//
// - On the y axis, 0 is the top of the track, and coordinates increase
//   _downward_. The track has fixed height, so all
//   y coordinates are between 0 and SCREEN_HEIGHT.
//
// Besides coordinates, we also store width,height dimensions
// for each object. x,y,width,height define a rectangle, so
// they are stored together in obj->rect of type Rectangle (defined
// in include/raylib.h). x,y refer to the top-left corner of the Rectangle.

static void add_objects(State state, float start_x) {
	// Add PLATFORM_NUM platforms with random attributes.

	for (int i = 0; i < PLATFORM_NUM; i++) {
		Object platform = create_object(
			PLATFORM,
			start_x + 150 + rand() % 80,						// x with random gap from previous in [150, 230]
			SCREEN_HEIGHT / 4 + rand() % SCREEN_HEIGHT / 2,		// y randomly in [SCREEN_HEIGHT/4, 3*SCREEN_HEIGHT/4]
			i == 0 ? 250 : 50 + rand() % 200,						// width randomly in [50, 250] (first is always 250)
			20,													// height
			i < 3 || rand() % 2 == 0 ? MOVING_UP : MOVING_DOWN,	// random initial movement (first 3 always upward)
			0.6 + 3 * (rand() % 100) / 100,						// speed randomly in [0.6, 3.6]
			i > 0 && (rand() % 5) == 0							// 20% (randomly) of platforms are unstable (except first)
		);
		vector_insert_last(state->objects, platform);

		// On 50% of platforms (randomly), except the first, add a star
		if (i != 0 && rand() % 2 == 0) {
			Object star = create_object(
				STAR,
				start_x + 200 + rand() % 60,				// x with random distance from previous platform in [200,260]
				SCREEN_HEIGHT / 8 + rand() % SCREEN_HEIGHT / 2,	// y randomly in [SCREEN_HEIGHT/8, 5*SCREEN_HEIGHT/8]
				30,
				30,										// width, height
				IDLE,										// no movement
				0,									 	// speed 0
				false										// 'unstable' always false for stars
			);
			vector_insert_last(state->objects, star);
		}

		start_x = platform->rect.x + platform->rect.width;	// shift next objects to the right
	}
}

// Creates and returns the initial game state

State state_create() {
	// Create state
	State state = malloc(sizeof(*state));

	// General information
	state->info.playing = true;			// The game starts immediately
	state->info.paused = false;			// Not paused
	state->speed_factor = 1;				// Normal speed
	state->info.score = 0;				// Initial score 0

	// Create the objects vector and add objects
	// starting from start_x = 0.
	state->objects = vector_create(0, NULL);
	add_objects(state, 0);

	// Create the ball and place it on top of the first platform
	Object first_platform = vector_get_at(state->objects, 0);
	state->info.ball = create_object(
		BALL,
		first_platform->rect.x,			// x at platform start
		first_platform->rect.y - 60,	// y above platform
		45, 60,							// width, height
		IDLE,							// no initial vertical movement
		0,								// initial speed 0
		false							// "unstable" always false for the ball
	);

	return state;
}

// Returns basic game information in state

StateInfo state_info(State state) {
	return &(state->info);
}

// Returns a list with all game objects in state
// whose x coordinate is between x_from and x_to.

List state_objects(State state, float x_from, float x_to) {
	List objects_list = list_create(NULL);

	for (int i = 0; i < vector_size(state->objects); i++) {
		Object obj1 = vector_get_at(state->objects, i);

		if (obj1->rect.x <= x_to && obj1->rect.x >= x_from) {
			list_insert_next(objects_list, LIST_BOF, obj1);
		}
	}

	return objects_list;
}

// Updates game state after one frame.
// keys contains the keys pressed during this frame.

// Due to the function's complexity, comments in UPPERCASE mark the section behavior,
// while lowercase comments explain details.
void state_update(State state, KeyState keys) {
	Object ball = state->info.ball;
	float horizontal_speed = ball->vert_mov == IDLE ? BALL_GROUND_SPEED : BALL_AIR_SPEED;

	// Logic runs when game is active and not paused, OR
	// while paused if n is pressed
	if (((state->info.playing == true) && (state->info.paused == false)) ||
		((state->info.playing == true) && (keys->n) && (state->info.paused == true))) {

		//BALL HORIZONTAL MOVEMENT
		if (keys->right) {
			ball->rect.x += horizontal_speed + BALL_RIGHT_SPEED_BONUS;
		} else if (keys->left) {
			// move slightly backward
			ball->rect.x -= BALL_LEFT_SPEED;
		} else {
			ball->rect.x += horizontal_speed;
		}

		//BALL VERTICAL MOVEMENT
		if (ball->vert_mov == JUMPING) {
			// Move upward by as many pixels as current vertical speed.
			ball->rect.y -= ball->vert_speed;
			ball->vert_speed *= BALL_JUMP_DECAY;
			if (ball->vert_speed <= 0.5) {
				ball->vert_mov = FALLING;
			}
		} else if (ball->vert_mov == FALLING) {
			ball->rect.y += ball->vert_speed;
			ball->vert_speed *= BALL_FALL_ACCELERATION;
			if (ball->vert_speed > BALL_MAX_FALL_SPEED) {
				ball->vert_speed = BALL_MAX_FALL_SPEED;
			}
		}
		// If the ball is idle and up is pressed, enter jump state with initial speed 17.
		else if (ball->vert_mov == IDLE) {
			if (keys->up) {
				ball->vert_mov = JUMPING;
				ball->vert_speed = BALL_JUMP_START_SPEED;
			}
		}


		// LOOP FOR EXERCISE 3 FEATURES + PLATFORM VERTICAL MOVEMENT FROM EXERCISE 2
		Vector current_objects = state->objects;
		Object last_platform = NULL;
		float max_x = 0;
		bool ball_onto_a_platform = false;

		for (int i = 0; i < vector_size(current_objects); i++) {
			// object currently accessed at each iteration
			Object obj_temp = vector_get_at(current_objects, i);

			if (obj_temp->type == PLATFORM) {
				//(EXERCISE 2) PLATFORM VERTICAL MOVEMENT
				if (obj_temp->vert_mov == MOVING_UP) {
					// Move upward by as many pixels as current vertical speed
					obj_temp->rect.y -= obj_temp->vert_speed;
					if (obj_temp->rect.y < SCREEN_HEIGHT / 4) {
						// since we move upward, y gets increasingly smaller, hence <
							obj_temp->vert_mov = MOVING_DOWN;
					}
				} else if (obj_temp->vert_mov == MOVING_DOWN) {
					obj_temp->rect.y += obj_temp->vert_speed;
					if (obj_temp->rect.y > 3 * SCREEN_HEIGHT / 4) {
							obj_temp->vert_mov = MOVING_UP;
					}
				} else if (obj_temp->vert_mov == FALLING) {
					obj_temp->rect.y += 4;
				}
				// in case of ground collision
				if (obj_temp->rect.y >= SCREEN_HEIGHT) {
					// move obj_temp to the end of the vector
					vector_swap(current_objects, i);
					vector_remove_last(current_objects);
				}
				// find platform with max x, which is the last platform
				if (max_x < obj_temp->rect.x) {
					max_x = obj_temp->rect.x;
					last_platform = obj_temp;
				}
			}
			//BALL COLLISIONS - (STAR OR PLATFORM)
			if (obj_temp->type == PLATFORM) {
				if ((ball->vert_mov == FALLING || ball->vert_mov == IDLE)
					&& ball_can_snap_to_platform(ball, obj_temp)) {
					ball->rect.y = obj_temp->rect.y - (ball->rect.height - 2);
					ball->vert_mov = IDLE;
					ball->vert_speed = 0;
					ball_onto_a_platform = true;

					if (obj_temp->unstable) {
						obj_temp->vert_mov = FALLING;
					}
				}
			} else if (CheckCollisionRecs(ball->rect, obj_temp->rect)) {
				if (obj_temp->type == STAR) {
					vector_swap(current_objects, i);
					vector_remove_last(current_objects);
					state->info.score += 10;
				}
			}
		}

		// If not standing on any platform
		if ((ball_onto_a_platform == false) && (ball->vert_mov == IDLE)) {
			ball->vert_mov = FALLING;
			ball->vert_speed = 1.5;
		}

		// BALL COLLISION WITH BOTTOM OF SCREEN
		// Since ball coordinates refer to the top-left corner, subtract ball height
		// so player loses when the bottom of the ball touches ground, not when ball disappears.
		if (ball->rect.y >= SCREEN_HEIGHT - ball->rect.height) {
			state->info.playing = false;
		}

		// When the ball reaches one-screen distance from the last platform,
		// add objects and increase speed factor by 10%
		if (last_platform->rect.x - ball->rect.x <= SCREEN_WIDTH) {
			add_objects(state, last_platform->rect.x + last_platform->rect.width);
		}

	// end of logic that runs only while playing or when n is pressed
	}
	
	//START AND STOP
	//If game is over and enter is pressed, restart from the beginning.
	if ((!state->info.playing) && (keys->enter)) {
		state_reset(state);
	}
	//pause
	if (keys->p) {
		if (!state->info.paused) {
			state->info.paused = true;
		} else {
			// if paused and p is pressed, resume playing
			state->info.paused = false;
		}
	}
}

// Destroys state and frees allocated memory.

void state_destroy(State state) {
	vector_set_destroy_value(state->objects, destroy_object);
	vector_destroy(state->objects);
	destroy_object(state->info.ball);
	free(state);
}
