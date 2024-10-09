#ifndef LEVEL_LOADER_
#define LEVEL_LOADER_

#include "custom_structs.h"
#include "../include/game_defines.h"
#include "game_defines.h"

char load_file(const char*, char [ROWS][COLS]);
void generate_level(const char*, Tile[ROWS][COLS],
                    Entity[ENTITY_TYPES][ENTITY_COUNT], Player *player);
void print_level(Tile print_level[ROWS][COLS]);

#endif // LOAD_LEVEL_
