#include "raylib.h"

#include "state.h"
#include "interface.h"

// Assets
Texture ball_img;
Texture star_img;
Sound game_over_snd;

void interface_init() {
	// Initialize window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
	InitAudioDevice();

	// Load images and sounds
	ball_img = LoadTextureFromImage(LoadImage("assets/ball.png"));
	star_img = LoadTextureFromImage(LoadImage("assets/star.png"));
	game_over_snd = LoadSound("assets/game_over.mp3");
}

void interface_close() {
	CloseAudioDevice();
	CloseWindow();
}

// Draw game (one frame)
void interface_draw_frame(State state) {
	BeginDrawing();

	// Clear screen, we will draw everything from scratch
	ClearBackground(RAYWHITE);

	// Draw the ball
	StateInfo info = state_info(state);
	
	float ball_x = info->ball->rect.x;
	float x_offset = (SCREEN_WIDTH / 2) - 200 - ball_x;

	// Offset is a constant that transforms a map variable into a screen variable when added
	DrawTexture(ball_img, ball_x + x_offset, info->ball->rect.y, WHITE);
	
	// Roughly draw everything around the ball: one screen ahead and one behind
	List objs = state_objects(state, ball_x - SCREEN_WIDTH, ball_x + SCREEN_WIDTH);

	for (ListNode node = list_first(objs);
		node != LIST_EOF;
		node = list_next(objs, node)) {
		Object obj = list_node_value(objs, node);

		if (obj->type == PLATFORM) {
			// screen y equals game-world y
			// compute x using offset
			DrawRectangle(obj->rect.x + x_offset + 5, obj->rect.y + 5, obj->rect.width, 20, Fade(BLACK, 0.3f));
			if (!obj->unstable) {
				// draw stable platforms
				DrawRectangle(obj->rect.x + x_offset, obj->rect.y, obj->rect.width, 20, MAROON); // Try MAROON instead of plain RED
			} else {
				// draw unstable platforms
				DrawRectangle(obj->rect.x + x_offset, obj->rect.y, obj->rect.width, 20, GRAY);
			}
		} else if (obj->type == STAR) {
			// DrawRectangle( obj->rect.x + x_offset  , obj->rect.y ,obj->rect.width,20 , YELLOW);
			DrawTexture(star_img, obj->rect.x + x_offset, obj->rect.y, WHITE);
		}
	}

	// Draw score and FPS counter
	DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// If game is over, draw restart message
	if (!(state_info(state)->playing) && (!state_info(state)->paused)) {
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			GetScreenHeight() / 2 - 50,
			20,
			GRAY
		);
	} else if (state_info(state)->paused) {
		DrawText(
			"PRESS [P] TO CONTINUE",
			GetScreenWidth() / 2 - MeasureText("PRESS [P] TO CONTINUE", 20) / 2,
			GetScreenHeight() / 2 - 50,
			20,
			GRAY
		);
	}
	EndDrawing();
}