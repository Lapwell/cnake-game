#ifndef GAME_DEFINES_
#define GAME_DEFINES_

// General defines
#define BIG_FONT 40
#define SMALL_FONT 20
#define WINDOWH 1024
#define WINDOWW 1024

// Level tile defines (types, size, how many)
#define WALL '#'
#define PLAYER '@'
#define HAZARD '&'
#define EXIT '^'
#define FRUIT '$'
#define POWERUP '%'
#define ENEMY '!'
#define FLOOR '.'
#define TILE_SIZE 32
#define ROWS 32
#define COLS 32

// Player related defines
#define SNAKE_SIZE 32
#define DIRECTION 1
#define STILL 0
#define UP 1
#define DOWN 3
#define LEFT 4
#define RIGHT 2

// Entities
#define ENTITY_COUNT 8
#define ENTITY_TYPES 4

#endif // !GAME_DEFINES_
