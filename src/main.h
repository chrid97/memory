#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t faceup_tile_count;
} GameState;

typedef struct {
  float height;
  float width;
  Vector2 pos;
  int card_value;
  bool is_face_up;
  char tile_value[32];
} Entity;

typedef enum { FaceDown, FaceUp, Scored } TileState;
