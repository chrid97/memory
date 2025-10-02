#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_TILES 100

typedef enum { FaceDown, FaceUp, Scored } TileState;
typedef enum { GAME_OVER, PLAYING } State;

typedef struct {
  float height;
  float width;
  Vector2 pos;
  TileState state;
  char tile_value[32];
} Entity;

typedef struct {
  int prev_flipped_tile_index;
  int current_flipped_tile_index;
  int score;
  int level;
  Entity tiles[MAX_TILES];
  int tiles_count;
  int player_health;
  State state;
} GameState;
