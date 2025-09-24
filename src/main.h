#include <raylib.h>
#include <stdbool.h>

typedef struct {
} GameState;

typedef struct {
  float height;
  float width;
  Vector2 pos;
  int card_value;
  bool is_face_up;
  char tile_value[32];
} Entity;
