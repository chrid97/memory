#include "main.h"
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int VIRTUAL_WIDTH = 800;
const int VIRTUAL_HEIGHT = 450;
const int TILE_WIDTH = 70;
const int TILE_HEIGHT = 70;
const int DEFAULT_PLAYER_MOVES = 5;
const int SIDEBAR_WIDTH = 200;
const int PLAYAREA_WIDTH = VIRTUAL_WIDTH - SIDEBAR_WIDTH;

bool DEBUG = 0;

void test(char *text) {
  DrawText(text, GetScreenWidth() / 2, GetScreenHeight() / 2, 100, RED);
}

int random_between(int min, int max) { return min + rand() % (max - min + 1); }

int main(void) {
  InitWindow(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, "Memory");
  SetTargetFPS(60);

  GameState game_state = {.prev_flipped_tile_index = 103,
                          .current_flipped_tile_index = 103,
                          .level = 1,
                          .moves = DEFAULT_PLAYER_MOVES,
                          .state = PLAYING};

  // (TODO) Move this into game_state
  Entity tiles[MAX_TILES];
  int tiles_count = 20;

  // position tiles
  int gap = 5;
  int cols = 5;
  // Center tiles in the play area
  float center_x =
      (PLAYAREA_WIDTH / 2.0f) - ((TILE_WIDTH * 2.0f) + (TILE_WIDTH / 2.0f));
  float center_y = (VIRTUAL_HEIGHT / 2.0f) - (TILE_HEIGHT * 2.0f);
  for (int i = 0; i < tiles_count; i++) {
    int row = i / cols;
    int col = i % cols;
    int x = (float)SIDEBAR_WIDTH + ((TILE_WIDTH + gap) * col);
    int y = (TILE_HEIGHT + gap) * row;
    tiles[i] = (Entity){.pos = {.x = x + center_x, .y = y + center_y},
                        .width = TILE_WIDTH,
                        .height = TILE_HEIGHT,
                        .state = FaceDown};
  }

  float flip_timer = 0.0f;
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    // Screen scaling
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    float delta = GetFrameTime();
    float scale_x = (float)screen_width / VIRTUAL_WIDTH;
    float scale_y = (float)screen_height / VIRTUAL_HEIGHT;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;

    // --------------- //
    // ---- Input ---- //
    // --------------- //
    if (IsKeyPressed(KEY_O)) {
      DEBUG = !DEBUG;
    }

    // ---------------- //
    // ---- Update ---- //
    // ---------------- //

    if (flip_timer > 0) {
      flip_timer -= dt;
      if (flip_timer <= 0) {
        flip_timer = 0;
      }
    }

    if (game_state.moves == 0) {
      game_state.state = GAME_OVER;
    }

    if (game_state.state == GAME_OVER) {
      // RESET THE GAME
      game_state.moves = DEFAULT_PLAYER_MOVES;
      game_state.level = 1;
      game_state.state = PLAYING;
      game_state.score = 0;
      // update_level(tiles, tiles_count);
    }

    // (TODO) Prevent clicking on a tile when you have two flip already
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && flip_timer == 0) {
      for (int i = 0; i < tiles_count; i++) {
        Entity *tile = &tiles[i];
        Vector2 mouse_pos = GetMousePosition();
        Rectangle rect = {tile->pos.x * scale, tile->pos.y * scale,
                          tile->width * scale, tile->height * scale};
        if (CheckCollisionPointRec(mouse_pos, rect) &&
            tile->state == FaceDown) {
          tile->state = FaceUp;

          if (game_state.prev_flipped_tile_index == 103) {
            game_state.prev_flipped_tile_index = i;
          } else {
            game_state.current_flipped_tile_index = i;
            flip_timer = 0.5f;
          }
        }
      }
    };

    bool all_tiles_scored = true;
    for (int i = 0; i < tiles_count; i++) {
      if (tiles[i].state != Scored) {
        all_tiles_scored = false;
        break;
      }
    }

    if (all_tiles_scored) {
      game_state.level++;
      // update_level(tiles, tiles_count);
      all_tiles_scored = false;
    }

    // ---------------- //
    // ----- Draw ----- //
    // ---------------- //
    BeginDrawing();
    ClearBackground(DARKBLUE);

    // Sidebar
    Color grey = {68, 68, 78, 255};
    DrawRectangle(0, 0, (float)SIDEBAR_WIDTH * scale, screen_height, grey);

    // Tiles
    for (int i = 0; i < tiles_count; i++) {
      Entity *tile = &tiles[i];
      if (tile->state == FaceDown) {
        DrawRectangle(tile->pos.x * scale, tile->pos.y * scale,
                      tile->width * scale, tile->height * scale, GREEN);
      } else {
        DrawRectangle(tile->pos.x, tile->pos.y, tile->width, tile->height,
                      WHITE);
      }
    }

    int font_size = 25 * scale;
    const char *score = TextFormat("Score: %i", game_state.score);
    DrawText(score, 0, scale, font_size, YELLOW);
    const char *level = TextFormat("Level: %i", game_state.level);
    DrawText(level, 0, 30 * scale, font_size, YELLOW);
    const char *player_moves = TextFormat("Moves: %i", game_state.moves);
    DrawText(player_moves, 0, 60 * scale, font_size, YELLOW);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
