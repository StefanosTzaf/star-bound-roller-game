//////////////////////////////////////////////////////////////////
//
// Test for state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Simple library for unit testing

#include "state.h"


void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);
	 
	TEST_ASSERT(info->ball->rect.width == 45);
	
	// Initially, ball should be between these coordinates
	TEST_ASSERT(info->ball->rect.x <= 230);
	TEST_ASSERT(info->ball->rect.x >=150);
	
	TEST_ASSERT(info->ball->rect.y >= SCREEN_HEIGHT/4 -40);
	TEST_ASSERT(info->ball->rect.y <= 3*(SCREEN_HEIGHT/4) -40);


}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Key information (initially no key is pressed)
	struct key_state keys = { false, false, false, false, false, false, false };
	
	// Without any key, ball moves 4 pixels to the right
	Rectangle old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->ball->rect;
	TEST_ASSERT( new_rect.x == old_rect.x + 4 );

	// With right arrow pressed, ball moves 6 pixels to the right
	keys.right = true;
	old_rect = state_info(state)->ball->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->ball->rect;

	TEST_CHECK( new_rect.x == old_rect.x + 6 );



	keys.right = false;
	
	// Set an impossible value to avoid collision with any platform and test only
	// what we want
	state_info(state)->ball->rect.y =-50;
	state_info(state)->ball->vert_mov = JUMPING;
	state_info(state)->ball->vert_speed = 0.55;
	state_update(state,&keys);
	// If ball is in jumping state and its speed (after becoming 85% of its value)
	// is below 0.5, it should switch to falling state
	TEST_ASSERT(state_info(state)->ball->vert_mov == FALLING);


	// If in IDLE and up arrow is pressed, it enters JUMPING with initial speed 17.
	state_info(state)->ball->vert_mov = IDLE;
	keys.up = true;
	state_update(state, &keys);
	state_info(state)->ball->rect.x = -10; // so state does not change due to platform collision
	TEST_ASSERT(state_info(state)->ball->vert_speed == 17);
	TEST_ASSERT(state_info(state)->ball->vert_mov == JUMPING);


	// If ball reaches bottom of screen, game ends.
	 state_info(state)->playing = true ;
	 state_info(state)->ball->rect.y = SCREEN_HEIGHT;
	 state_update(state,&keys);
	 TEST_ASSERT(state_info(state)->playing == false);

}

void test_state_objects(){
	State state = state_create();

	// After one call to state_create, 20 platforms are always created and possibly one star on each (except
	// the first), so at best 20+19; state_objects should return at most 39 when given
	// total width (20*900)
	List objs =state_objects(state,0,20*900);
	TEST_ASSERT(objs != NULL);
	TEST_ASSERT(list_size(objs)<=39);
	
	for(ListNode node=list_first(objs);
		node !=LIST_EOF;
	 	node=list_next(objs,node) ){
		// Check that objects returned in the list are indeed within x values [0, 20*900]
		Object obj = list_node_value(objs,node);
		TEST_ASSERT(obj->rect.x <= 20*900);
		TEST_ASSERT(obj->rect.x >=0);
	}
	
	// At most 3 platforms and 2 stars in the first screen where SCREEN_WIDTH=900
	List objs1 =state_objects(state,0,900);
	TEST_ASSERT(objs != NULL);
	TEST_ASSERT(list_size(objs1)<=5);
}


// List of all tests to execute
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },
	{"test_state_objects", test_state_objects},

	{ NULL, NULL } // terminate list with NULL
};