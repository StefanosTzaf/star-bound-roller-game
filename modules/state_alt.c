
#include <stdlib.h>
#include "state.h"
#include "set_utils.h"

// If object a has larger x coordinate, return positive; otherwise negative
int objs_compare(Pointer a,Pointer b){
	Object a1 = a;
    Object b1 = b;
	if(a1->rect.x> b1->rect.x){
        return 1;
    }
    else if(a1->rect.x < b1->rect.x){
        return -1;
    }
	return 0;
}

struct state {
	Set objects;			// Contains Object elements (platforms, stars)
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

	Set old_objects = state->objects;
	Object old_ball = state->info.ball;

	state->objects = new_state->objects;
	state->info = new_state->info;
	state->speed_factor = new_state->speed_factor;

	free(new_state);
	set_set_destroy_value(old_objects, destroy_object);
	set_destroy(old_objects);
	destroy_object(old_ball);
}

// Creates and returns an object

static Object create_object(ObjectType type, float x, float y, float width, float height, VerticalMovement vert_mov, float speed, bool unstable) {
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
			SCREEN_HEIGHT/4 + rand() % SCREEN_HEIGHT/2,			// y randomly in [SCREEN_HEIGHT/4, 3*SCREEN_HEIGHT/4]
			i == 0 ? 250 : 50 + rand()%200,						// width randomly in [50, 250] (first is always 250)
			20,													// height
			i < 3 || rand() % 2 == 0 ? MOVING_UP : MOVING_DOWN,	// random initial movement (first 3 always upward)
			0.6 + 3*(rand()%100)/100,							// speed randomly in [0.6, 3.6]
			i > 0 && (rand() % 5) == 0							// 20% (randomly) of platforms are unstable (except first)
		);
		set_insert(state->objects, platform);

		// On 50% of platforms (randomly), except the first, add a star
		if(i != 0 && rand() % 2 == 0) {
			Object star = create_object(
				STAR,
				start_x + 200 + rand() % 60,				// x with random distance from previous platform in [200,260]
				SCREEN_HEIGHT/8 + rand() % SCREEN_HEIGHT/2,	// y randomly in [SCREEN_HEIGHT/8, 5*SCREEN_HEIGHT/8]
				30, 30,										// width, height
				IDLE,										// no movement
				0,									 	// speed 0
				false										// 'unstable' always false for stars
			);
			set_insert(state->objects, star);
		}

		start_x = platform->rect.x + platform->rect.width;	// shift next objects to the right
	}
}

// Creates and returns the initial game state

State state_create() {
	// Create state
	State state = malloc(sizeof(*state));

	// General information
	state->info.playing = true;				// The game starts immediately
	state->info.paused = false;				// Not paused
	state->speed_factor = 1;				// Normal speed
	state->info.score = 0;				// Initial score 0

	// Create the objects set and add objects
	// starting from start_x = 0.
	state->objects = set_create(objs_compare, NULL);
	add_objects(state, 0);

	// Create the ball and place it on top of the first platform
	Object first_platform = set_node_value(state->objects,set_first(state->objects));// first platform is at the beginning of the set
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

StateInfo state_info(State state) {
	return &(state->info);
}

// Returns a list with all game objects in state,
// whose x coordinate is between x_from and x_to.

List state_objects(State state, float x_from, float x_to) {
	
	List objects_list=list_create(NULL);

	// Create two objects with x = x_from and x = x_to respectively
	Object obj_temp1 = create_object(PLATFORM,x_from,0,50,20,FALLING,1.0,false);
	Object obj_temp2 = create_object(PLATFORM,x_to,0,50,20,FALLING,1.0,false);


	// Get the first set object between x_from and x_to
	Object obj_first = set_find_eq_or_greater(state->objects , obj_temp1);
	// and its corresponding node
	SetNode first_node = set_find_node(state->objects,obj_first);
	
	
	// Get the last set object between x_from and x_to
	Object obj_last = set_find_eq_or_smaller(state->objects,obj_temp2);
	// and its node
	SetNode last_node = set_find_node(state->objects,obj_last);

	// If obj_first is the largest set element OR obj_last is the smallest,
	// there cannot be set elements between them

	if((obj_first == NULL) || (obj_last == NULL)){
		free(obj_temp1);
		free(obj_temp2);
		return objects_list;
	}
	else if(first_node == last_node){
		list_insert_next(objects_list,LIST_BOF,obj_first);
		free(obj_temp1);
		free(obj_temp2);
		return objects_list;
	}
	
	

	SetNode current_node = first_node;
	Object current_object = set_node_value(state->objects,current_node);
	// Using set ordering, append elements to the list
	// up to last
	while(current_node != last_node){
		list_insert_next(objects_list,LIST_BOF,current_object);

		current_node = set_next(state->objects,current_node);
		current_object = set_node_value(state->objects,current_node);
	}
	list_insert_next(objects_list,LIST_BOF,obj_last);
	free(obj_temp1);
	free(obj_temp2);

	return objects_list;
}







void state_update(State state, KeyState keys) {
	Object ball=state->info.ball;
	float horizontal_speed = ball->vert_mov == IDLE ? BALL_GROUND_SPEED : BALL_AIR_SPEED;
	if( ((state->info.playing == true) && (state->info.paused == false)) || ((state->info.playing==true) &&(keys->n) &&(state->info.paused == true))){
		//BALL HORIZONTAL MOVEMENT
		if(keys->right){
			ball->rect.x += horizontal_speed + BALL_RIGHT_SPEED_BONUS;
		}
		else if(keys->left){
			ball->rect.x -= BALL_LEFT_SPEED;
		}
		else{
			ball->rect.x += horizontal_speed;
		}


		//BALL VERTICAL MOVEMENT
		if( (ball->vert_mov) == JUMPING ){  
			// Move upward by as many pixels as current vertical speed.
			ball->rect.y -= ball->vert_speed ;
			ball->vert_speed *= BALL_JUMP_DECAY;
			if(ball->vert_speed <= 0.5){
				ball->vert_mov = FALLING;
			}
		}
		else if( (ball->vert_mov) == FALLING ){
			ball->rect.y += ball->vert_speed;
			ball->vert_speed *= BALL_FALL_ACCELERATION;
			if( ball->vert_speed > BALL_MAX_FALL_SPEED ){
				ball->vert_speed = BALL_MAX_FALL_SPEED;
			}
		}
		// If the ball is idle and up is pressed, enter jump state with initial speed 17.
		else if( (ball->vert_mov) == IDLE ){
			if(keys->up){
				ball->vert_mov = JUMPING;
				ball->vert_speed = BALL_JUMP_START_SPEED;
			}
		}

		

		Set current_objects = state->objects;
		// object with x two screens behind, since we check objects within a two-screen distance
		Object two_screens_back = create_object(PLATFORM,ball->rect.x - 2*SCREEN_WIDTH,200,250,80,MOVING_UP,1.1,true);
		// take the immediately greater set object than the one above
		Object first_obj = set_find_eq_or_greater(current_objects,two_screens_back);
		SetNode node_first = first_obj == NULL ? SET_EOF : set_find_node(current_objects,first_obj);
		free(two_screens_back);

		Object last_platform = NULL;
		float max_x = 0;
		bool ball_onto_a_platform = false;

		// LOOP FOR EXERCISE 3 FEATURES + PLATFORM VERTICAL MOVEMENT FROM EXERCISE 2
		// for objects from two screens before to two screens after the ball
		SetNode node = node_first;
		while(node != SET_EOF) {
			Object obj_temp = set_node_value(current_objects,node);
			if(obj_temp->rect.x > ball->rect.x + SCREEN_WIDTH*2){
				break;
			}

			SetNode next_node = set_next(current_objects, node);
			// Ball behavior while vertically idle
			if(obj_temp->type == PLATFORM){
				//(EXERCISE 2) PLATFORM VERTICAL MOVEMENT
				if(obj_temp->vert_mov == MOVING_UP){
				// Move upward by as many pixels as current vertical speed
					obj_temp->rect.y -= obj_temp->vert_speed;
						if(obj_temp->rect.y < SCREEN_HEIGHT/4){ 
						// since we move upward, y gets increasingly smaller, hence <
							obj_temp->vert_mov = MOVING_DOWN;
						}
				}
				else if(obj_temp->vert_mov == MOVING_DOWN){
					obj_temp->rect.y += obj_temp->vert_speed;
						if(obj_temp->rect.y > 3*SCREEN_HEIGHT/4){ 
							obj_temp->vert_mov = MOVING_UP;
						}		
				}
				else if (obj_temp->vert_mov == FALLING){
					obj_temp->rect.y += 4;

				}
				// in case of ground collision
				if(obj_temp->rect.y >= SCREEN_HEIGHT){
					set_remove(current_objects, obj_temp);
					node = next_node;
					continue;
				}
				
				if(max_x < obj_temp->rect.x){
					max_x = obj_temp->rect.x;
					last_platform = obj_temp;
				}
			}
			if(obj_temp->type == PLATFORM){
				if((ball->vert_mov == FALLING || ball->vert_mov == IDLE)
					&& ball_can_snap_to_platform(ball, obj_temp)) {
					ball->rect.y = obj_temp->rect.y - (ball->rect.height - 2);
					ball->vert_mov = IDLE;
					ball->vert_speed = 0;
					ball_onto_a_platform = true;

					if(obj_temp->unstable){
						obj_temp->vert_mov = FALLING;
					}
				}
			}
			else if(CheckCollisionRecs(ball->rect,obj_temp->rect)){
				if (obj_temp->type == STAR){ 
					set_remove(current_objects, obj_temp);
					state->info.score +=10;
					node = next_node;
					continue;
				}	
			}

			node = next_node;
		} 	
		// If not standing on any platform
		if((ball_onto_a_platform == false) && (ball->vert_mov == IDLE)){
			ball->vert_mov = FALLING ;
			ball->vert_speed = 1.5 ;
		}
		

		// BALL COLLISION WITH BOTTOM OF SCREEN
		// Since ball coordinates refer to the top-left corner, subtract ball height
		// so player loses when the bottom of the ball touches ground, not when ball disappears.
		if(ball->rect.y >= SCREEN_HEIGHT - ball->rect.height){
			state->info.playing = false ; 
		}

		if(last_platform != NULL && last_platform->rect.x - ball->rect.x <= SCREEN_WIDTH ){
		 	add_objects(state,last_platform->rect.x + last_platform->rect.width);
			
		}
		// end of logic that runs only while playing or when n is pressed
	}
	
	//START AND STOP
	//If game is over and enter is pressed, restart from the beginning.
	if((!state->info.playing) && (keys->enter)){
		state_reset(state);
	}
	//pause
	if(keys->p){
		if(!state->info.paused){
			state->info.paused = true;
		}
		// if paused and p is pressed, resume playing
		else {
			state->info.paused = false;
		}
	}
}

// Destroys state and frees allocated memory.

void state_destroy(State state) {
	set_set_destroy_value(state->objects, destroy_object);
	set_destroy(state->objects);
	destroy_object(state->info.ball);
	free(state);
}
