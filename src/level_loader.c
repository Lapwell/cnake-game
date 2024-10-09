#include "../include/level_loader.h"
#include "../include/game_defines.h"
#include "../include/custom_structs.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

// TODO: Look into returning a second 2d array that stores the location for fruits,
// enemies, and powerups (PUs might be finite).

int entityArrLength(Entity arr[]) {
  int count = 0;
  while (arr[count].type != '\0') {
    count++;
  }
  return count;
}

void print_level(Tile print_level[ROWS][COLS]) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%c", print_level[i][j].type);
    }
    printf("\n");
  }
}

char load_file(const char *file_name, char raw_level[ROWS][COLS]) {
  FILE *file;
  int row = 0;
  char buffer[35];
  file = fopen(file_name, "r");
  if (file == NULL) {
    printf("\nUnable open file\n");
    exit(1);
  }
  while (fgets(buffer, sizeof(buffer), file)) {
    for (size_t col = 0; col < sizeof(buffer); col++) {
      if (buffer[col] == '|' || buffer[col] == '\n' || buffer[col] == '\t') {
        //printf("Continue: %zi | Row: %i\n", col, row);
        continue;
      }
      raw_level[row][col] = buffer[col];
    }
    row++;
  }
  if (row > ROWS) {
    fclose(file);
    exit(1);
  }
  fclose(file);
  return **raw_level;
}

void generate_level(const char *file_name, Tile parsed_level[ROWS][COLS],
                    Entity entity_list[ENTITY_TYPES][ENTITY_COUNT], Player *player)
{
  char raw_level[ROWS][COLS];
  load_file(file_name, raw_level);
  int length;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      switch (raw_level[r][c]) {
        case WALL:
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, GRAY, WALL};
          break;
        case PLAYER:
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, WHITE, FLOOR};
          player->rect.x = TILE_SIZE * c;
          player->rect.y = TILE_SIZE * r;
          break;
        case EXIT:
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, BLUE, EXIT};
          break;
        case FRUIT:
          length = entityArrLength(entity_list[0]);
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, WHITE, FRUIT};
          entity_list[0][length] = (Entity){.type = FRUIT, .colour = LIME, .direction = STILL,
                                 .rect = {TILE_SIZE * c, TILE_SIZE *r, TILE_SIZE, TILE_SIZE}};
          break;
        case POWERUP:
          length = entityArrLength(entity_list[1]);
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, WHITE, POWERUP};
          entity_list[1][length] = (Entity){.type = POWERUP, .colour = PURPLE, .direction = STILL,
                                            .rect = {TILE_SIZE * c, TILE_SIZE *r, TILE_SIZE, TILE_SIZE}};
          break;
        case HAZARD:
          length = entityArrLength(entity_list[2]);
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, WHITE, HAZARD};
          entity_list[2][length] = (Entity){.type = POWERUP, .colour = YELLOW, .direction = STILL,
                                            .rect = {TILE_SIZE * c, TILE_SIZE * r, TILE_SIZE, TILE_SIZE}};
          break;
        case ENEMY:
          length = entityArrLength(entity_list[3]);
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, WHITE, ENEMY};
          entity_list[3][length] = (Entity){.type = ENEMY, .colour = RED, .direction = STILL,
                                            .rect = {TILE_SIZE * c, TILE_SIZE * r, TILE_SIZE, TILE_SIZE}};
          break;
        case FLOOR:
          parsed_level[r][c] = (Tile){{TILE_SIZE * c, TILE_SIZE * r,
                                       TILE_SIZE, TILE_SIZE}, WHITE, FLOOR};
          break;
      }
    }
  }
}

