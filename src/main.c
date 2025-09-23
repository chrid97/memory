#include "main.h"
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define VIRTUAL_WIDTH 800
#define VIRTUAL_HEIGHT 450
#define TILE_WIDTH 85
#define TILE_HEIGHT 100

void test(char *text) {
  DrawText(text, GetScreenWidth() / 2, GetScreenHeight() / 2, 100, RED);
}

void draw_tile(Entity *tile, float scale) {
  const char *text;
  if (tile->is_face_up) {
    text = "1";
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
  if (tile->is_face_up) {
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
  InitWindow(1920, 1080, "Tradebinder");
  SetTargetFPS(60);

  Entity tiles[4];
  int tiles_length = 4;
  for (int i = 0; i < tiles_length; i++) {
    int tile_gap = 25;
    tiles[i] = (Entity){
        .pos = (Vector2){.x = 10 + i * (TILE_WIDTH + tile_gap), .y = 25},
        .width = TILE_WIDTH,
        .height = TILE_HEIGHT,
        .is_face_up = false,
    };
  }

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

    // ---------------- //
    // ---- Update ---- //
    // ---------------- //
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      for (int i = 0; i < tiles_length; i++) {
        Entity *tile = &tiles[i];
        Vector2 mouse_pos = GetMousePosition();
        Rectangle rect = {tile->pos.x * scale, tile->pos.y * scale,
                          tile->width * scale, tile->height * scale};
        if (CheckCollisionPointRec(mouse_pos, rect)) {
          tile->is_face_up = true;
        }
      }
    };

    // ---------------- //
    // ----- Draw ----- //
    // ---------------- //
    BeginDrawing();
    ClearBackground(DARKBLUE);

    for (int i = 0; i < tiles_length; i++) {
      draw_tile(&tiles[i], scale);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
