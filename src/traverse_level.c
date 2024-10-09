#include "../include/traverse_level.h"

int* traverse_level(Tile level[ROWS][COLS], char locate) {
  int r;
  int c;
  static int location[2] = {};
  for (r = 0; r < ROWS; r++) {
    for (c = 0; c < COLS; c++) {
      if (level[r][c].type == locate) {
        location[0] = r;
        location[1] = c;
        return location;
      }
    }
  }
  location[0] = -1;
  location[1] = -1;
  return location;
}

