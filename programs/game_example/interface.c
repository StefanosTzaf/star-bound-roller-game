#include "raylib.h"

#include "state.h"
#include "interface.h"

// Assets
Texture bird_img;
Sound game_over_snd;


void interface_init() {
	// Initialize window
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game_example");
	SetTargetFPS(60);
    InitAudioDevice();

	// Load images and sounds
	bird_img = LoadTextureFromImage(LoadImage("assets/bird.png"));
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

	// Draw character and the 2 balls
	DrawTexture(bird_img, state->character.x, state->character.y, WHITE);
	DrawCircle(state->ball1.position.x, state->ball1.position.y, state->ball1.radius, RED);
	DrawCircle(state->ball2.position.x, state->ball2.position.y, state->ball2.radius, RED);

	// Draw score and FPS counter
	DrawText(TextFormat("%04i", state->score), 20, 20, 40, GRAY);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// If game is over, draw restart message
	if (!state->playing) {
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			 GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, GRAY
		);
	}

	// Sound, if this is the frame where game_over occurred
	if(state->game_over)
		PlaySound(game_over_snd);

	EndDrawing();
}