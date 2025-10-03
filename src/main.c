#include "main.h"
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define VIRTUAL_WIDTH 800
#define VIRTUAL_HEIGHT 450
#define TILE_WIDTH 85
#define TILE_HEIGHT 100
#define DEFAULT_PLAYER_HEALTH 5

bool DEBUG = 0;

void test(char *text) {
  DrawText(text, GetScreenWidth() / 2, GetScreenHeight() / 2, 100, RED);
}

int random_between(int min, int max) { return min + rand() % (max - min + 1); }

void draw_tile(Entity *tile, float scale) {
  const char *text;
  if (tile->state == FaceUp || tile->state == Scored) {
    text = tile->tile_value;
  } else {
    text = "?";
  }
  int font_size = 90;
  int text_width = MeasureText(text, font_size);
  int text_height = font_size;
  float rect_center_x = tile->pos.x + tile->width / 2.0f;
  float rect_center_y = tile->pos.y + tile->height / 2.0f;
  float textX = rect_center_x - text_width / 2.0f;
  float textY = rect_center_y - text_height / 2.0f;
  // A Scored card is a faceup card, i should probably chagne faceup to
  // something else
  if (tile->state == FaceUp || tile->state == Scored) {
    DrawRectangle(tile->pos.x * scale, tile->pos.y * scale, tile->width * scale,
                  tile->height * scale, WHITE);
    DrawText(text, textX * scale, textY * scale, font_size * scale, ORANGE);
  } else {
    DrawRectangle(tile->pos.x * scale, tile->pos.y * scale, tile->width * scale,
                  tile->height * scale, DARKGREEN);
    DrawText(text, textX * scale, textY * scale, font_size * scale, ORANGE);
  }
  DrawRectangleLinesEx(
      (Rectangle){
          .x = tile->pos.x * scale,
          .y = tile->pos.y * scale,
          .width = tile->width * scale,
          .height = tile->height * scale,
      },
      3, ORANGE);
}

int main(void) {
  InitWindow(1440, 1200, "Memory");
  SetTargetFPS(60);

  GameState game_state = {.prev_flipped_tile_index = 103,
                          .current_flipped_tile_index = 103,
                          .level = 1,
                          .player_health = DEFAULT_PLAYER_HEALTH,
                          .state = PLAYING};

  // (TODO) Move this into game_state
  Entity tiles[MAX_TILES];
  int tiles_count = 20;
  int gap = 5;
  int cols = 5;
  for (int i = 0; i < tiles_count; i++) {
    int row = i / cols;
    int col = i % cols;

    tiles[i] = (Entity){.pos = {.x = ((gap + TILE_WIDTH) * col),
                                .y = ((TILE_HEIGHT + gap) * row)},
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

    // compare cards
    if (game_state.current_flipped_tile_index != 103 && flip_timer == 0) {
      char *prev_tile = tiles[game_state.prev_flipped_tile_index].tile_value;
      char *current_tile =
          tiles[game_state.current_flipped_tile_index].tile_value;
      if (strcmp(prev_tile, current_tile) == 0) {
        tiles[game_state.prev_flipped_tile_index].state = Scored;
        tiles[game_state.current_flipped_tile_index].state = Scored;
        game_state.score++;
      } else {
        tiles[game_state.prev_flipped_tile_index].state = FaceDown;
        tiles[game_state.current_flipped_tile_index].state = FaceDown;
        game_state.player_health--;
      }
      game_state.prev_flipped_tile_index = 103;
      game_state.current_flipped_tile_index = 103;
    }

    if (game_state.player_health == 0) {
      game_state.state = GAME_OVER;
    }

    if (game_state.state == GAME_OVER) {
      // RESET THE GAME
      game_state.player_health = DEFAULT_PLAYER_HEALTH;
      game_state.level = 1;
      tiles_count = 2 * game_state.level;
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
      tiles_count = 2 * game_state.level;
      // update_level(tiles, tiles_count);
      all_tiles_scored = false;
    }

    // ---------------- //
    // ----- Draw ----- //
    // ---------------- //
    BeginDrawing();
    ClearBackground(DARKBLUE);

    for (int i = 0; i < tiles_count; i++) {
      draw_tile(&tiles[i], scale);
    }

    int health_container_width = 15 * scale;
    int health_container_height = 15 * scale;
    for (int i = 0; i < game_state.player_health; i++) {
      DrawRectangle((300 + health_container_width * i) * scale, 10 * scale,
                    health_container_width, health_container_width, RED);
    }

    // DEBUG //
    int font_size = 25 * scale;
    const char *score = TextFormat("Score: %i", game_state.score);
    DrawText(score, 20 * scale, scale, font_size, YELLOW);
    const char *level = TextFormat("Level: %i", game_state.level);
    DrawText(level, 20 * scale, 20 * scale, font_size, YELLOW);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
