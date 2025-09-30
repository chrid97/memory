#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TILES 100

typedef enum { FaceDown, FaceUp, Scored } TileState;

typedef struct {
  float height;
  float width;
  Vector2 pos;
  int card_value;
  TileState state;
  char tile_value[32];
} Entity;

typedef struct {
  uint8_t faceup_tile_count;
  int prev_flipped_tile_index;
  int current_flipped_tile_index;
  int score;
  int level;
  Entity tiles[MAX_TILES];
  int tiles_count;
  int player_health;
} GameState;
