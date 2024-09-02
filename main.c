#include "include/level_loader.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define TILE_SIZE 32
#define SNAKE_SIZE 32
#define UP 1
#define DOWN 3
#define LEFT 4
#define RIGHT 2
#define BIG_FONT 40

//--------------------------------
// Defining structs
//--------------------------------

typedef struct Player {
  int direction; // 1: up, 2: right, 3: down, 4:left
  int powerup; // This will be used for special abilities (invincibility, double
               // score, fire breath)
  Rectangle rect;
} Player;

//--------------------------------
// Global variables
//--------------------------------
Player player = {.direction = 2, .powerup = 0, .rect = {0, 0, SNAKE_SIZE, SNAKE_SIZE}};

int windowWidth = 1024, windowHeight = 1024;
char level_one[] = "level1.txt";

void gameOver() {
  BeginDrawing();
  ClearBackground(BLACK);
  int textWidth = MeasureText("GAME OVER", BIG_FONT);
  DrawText("GAME OVER", windowWidth / 2 - textWidth / 2, windowHeight / 3,
           BIG_FONT, RED);
  EndDrawing();
  WaitTime(4);
  CloseWindow();
}

void eventHandler() {
  if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
    player.direction = UP;
  } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
    player.direction = DOWN;
  } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
    player.direction = LEFT;
  } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
    player.direction = RIGHT;
  }
}

void updateGame() {
  switch (player.direction) {
  case UP:
    player.rect.y -= TILE_SIZE;
    break;
  case DOWN:
    player.rect.y += TILE_SIZE;
    break;
  case LEFT:
    player.rect.x -= TILE_SIZE;
    break;
  case RIGHT:
    player.rect.x += TILE_SIZE;
    break;
  }
  if (player.rect.y < 0 || player.rect.y > windowHeight) {
    gameOver();
  } else if (player.rect.x < 0 || player.rect.x > windowWidth) {
    gameOver();
  }
}

void initGame() {
  generateLevel(level_one);
}

int main() {
  float wait_time = 1.0f;
  float previous_time = 0.0f;
  InitWindow(windowWidth, windowHeight, "cnake game");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    float current_time = GetTime();
    BeginDrawing();
    eventHandler();
    ClearBackground(RAYWHITE);
    if (current_time - previous_time >= wait_time) {
      previous_time = GetTime();
      updateGame();
    }
    DrawRectangleRec(player.rect, GREEN);
    EndDrawing();
  }
  CloseWindow();
}
