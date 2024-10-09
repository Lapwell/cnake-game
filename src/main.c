#include "../include/level_loader.h"
#include "../include/game_defines.h"
#include "../include/custom_structs.h"
#include "handy_funcs.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO: Idea for next game: Adventures of Joe Poncho

#define WIN 0
#define LOSE 1
#define S_SCORE 10
#define B_SCORE 100

// MACROS
#define removeTile(remove_tile) (remove_tile.rect = (Tile){})

//--------------------------------
// Global variables
//--------------------------------

Player player = {.direction = 2, .powerup = 0, .body_len = 1,
                 .rect = {0, 0, SNAKE_SIZE, SNAKE_SIZE}};
Entity fruit;
Entity entities[ENTITY_TYPES][ENTITY_COUNT];
Tile level[ROWS][COLS];
Entity *snake_body;

int total_score = 0;
int level_score = 0;

char levels_list[4][32] = {"../levels/level1.txt", "..levels/level2.txt"};

/* gameOver()
 *
 * This function is for executing the game over screen, whether it's a win
 * or lose situation. It's stop the game and draws text befiting
 * the situation.
 *
 * Params:
 *    int end_state -> win = 0, lose = 1
 *    There are #defines for win/lose
 */
void gameOver(int end_state) {
  if (!end_state) {
    BeginDrawing();
    ClearBackground(BLACK);
    int text_width = MeasureText("YOU WIN", BIG_FONT);
    DrawText("YOU WIN", WINDOWW / 2 - text_width / 2, WINDOWH / 3,
              BIG_FONT, GREEN);
    EndDrawing();
    WaitTime(3);
    CloseWindow();
  } else {
    BeginDrawing();
    ClearBackground(BLACK);
    int text_width = MeasureText("GAME OVER", BIG_FONT);
    DrawText("GAME OVER", WINDOWW / 2 - text_width / 2, WINDOWH / 3,
              BIG_FONT, RED);
    EndDrawing();
    WaitTime(3);
    CloseWindow();
  }
}

/* displayUI()
 *
 * This guy is for displaying the current level score (some levels need a 
 * certain score to continue), displaying the global score
 * (total score across all levels), and the length of the snake (some levels
 * will need the player to long enough to continue).
 *
 * Params:
 *    int level_s -> level score
 *    int total_s -> total game score
 */
void displayUI(int level_s, int total_s) {
  //int level_s_width = MeasureText(level_s, SMALL_FONT);
  //int total_s_width = MeasureText(total_s, SMALL_FONT);
  DrawText(TextFormat("Score: %i", level_s), 10, 10, SMALL_FONT, BLACK);
  DrawText(TextFormat("Game Score: %i", total_s), 200, 10, SMALL_FONT, BLACK);
}

/* respawnEntity()
 *
 * This cheeki function is used to respawn various types of entities.
 * From the humble fruit to the hostile enemy, this function will respawn it.
 *
 * Params:
 *    Entity entity   -> the entity obj to respawn
 *    Tile valid_tile -> the tiles that the entity can spawn on.
 */
void respawnEntity(Entity *re_entity, char valid_tile) {
  int x_rand = GetRandomValue(0, COLS);
  int y_rand = GetRandomValue(0, ROWS);
  while (level[x_rand][y_rand].type != valid_tile) {
    x_rand = GetRandomValue(0, COLS);
    y_rand = GetRandomValue(0, ROWS);
  }
  re_entity->rect.x = level[x_rand][y_rand].rect.x;
  re_entity->rect.y = level[x_rand][y_rand].rect.y;
}

/* editSnake()
 *
 * This function handles extending and shrinking the snake's length.
 *
 * Params:
 *    bool grow -> true grows the snake by 1, false shrinks snake by 1.
 */
void editSnake(bool grow) {
  int length = player.body_len;
  // If we're growing the snake, grow it.
  if (grow) {
    snake_body = realloc(snake_body, (length + 1) * sizeof(Entity));
    if (snake_body == NULL) {
      printf("Error growing snake_body...");
      exit(1);
    }
    player.body_len++;
    length++;
    snake_body[length - 1].colour = GREEN;
    snake_body[length - 1].rect = (Rectangle) {-100, -100, SNAKE_SIZE, SNAKE_SIZE};
  } else {
    snake_body = realloc(snake_body, (length - 1) * sizeof(Entity));
    if (snake_body == NULL) {
      printf("Error shrinking snake_body...");
      exit(1);
    }
    player.body_len--;
    length--;
  }
}

/* eventHandler()
 *
 * This lil guy is for checking any game events. Player inputs, collision,
 * enemy attacks, spawning objects, and etc, is all done in here.
 *
 * Params:
 *    None
 */
void eventHandler() {
  // Check user input here
  if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
    player.direction = UP;
  } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
    player.direction = DOWN;
  } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
    player.direction = LEFT;
  } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
    player.direction = RIGHT;
  }

  // Check player collision with objects and do something depending
  // on what the object is.
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      Tile current_tile = level[r][c];
      if (CheckCollisionRecs(player.rect, current_tile.rect)) {
        switch (current_tile.type) {
          case WALL:
            printf("\nBONK!\n");
            gameOver(LOSE);
            break;
          case PLAYER:
            printf("\nHow????\n");
            gameOver(LOSE);
            break;
          case EXIT:
            printf("Exit reached\n");
            gameOver(WIN);
            break;
        }
      }
    }
  }

  // Checking for player collision with various entities
  for (int r = 0; r < ENTITY_TYPES; r++) {
    for (int c = 0; c < ENTITY_COUNT; c++) {
      Entity currenty_entity = entities[r][c];
      if (CheckCollisionRecs(player.rect, currenty_entity.rect)) {
        switch(currenty_entity.type) {
          case FRUIT:
            total_score += S_SCORE;
            level_score += S_SCORE;
            respawnEntity(&entities[r][c], FLOOR);
            editSnake(true);
            break;
          case POWERUP:
            total_score += B_SCORE;
            level_score += B_SCORE;
            respawnEntity(&entities[r][c], FLOOR);
            break;
          case HAZARD:
            gameOver(LOSE);
            break;
          case ENEMY:
            gameOver(LOSE);
            break;
        }
      }
    }
  }
}

/* updateGame()
 *
 * This function updates any logic or states. Entities moving, player movement,
 * checking animations (if I do any).
 *
 * Params:
 *    None
 */
void updateGame() {
  int length = player.body_len - 1;
  // Shift each body segment to the next position to make the body follow the player.
  for (int b = length; b >= 0; b--) {
    snake_body[b].rect = snake_body[b - 1].rect;
  }
  snake_body[0].rect = player.rect;

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

  // Checks if the player escapes the screen.
  if (player.rect.y < 0 || player.rect.y > WINDOWH) {
    gameOver(LOSE);
  } else if (player.rect.x < 0 || player.rect.x > WINDOWW) {
    gameOver(LOSE);
  }
}

/* initGame()
 *
 * This function is one of the first blocks of code executed
 * when the game starts. It loads the initial level from ./levels,
 * it sets the player to their starting position, spawns the first
 * set of fruits, powerups, hazards, enemies, etc.
 *
 * Params:
 *    None
 */
void initGame() {
  // First arg is the path to the level, second arg is where to store the data.
  generate_level(levels_list[0], level, entities, &player);
  snake_body = (Entity*)malloc(sizeof(Entity));
  snake_body[0].colour = GREEN;
  snake_body[0].rect = (Rectangle) {player.rect.x, player.rect.y,
                                    SNAKE_SIZE, SNAKE_SIZE};
}

/* main()
 *
 * The window is initialized here, the logic functions are called here, and
 * the game doing its thing is going on here.
 *
 * Params:
 *    None
 */
int main() {
  //float wait_time = 1.0f; // Easy
  //float wait_time = 0.64f; //Medium
  float wait_time = 0.25f; // Hard
  float previous_time = 0.0f;
  initGame();
  InitWindow(WINDOWW, WINDOWH, "Cnake Man");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    float current_time = GetTime();
    eventHandler();
    BeginDrawing();
    ClearBackground(WHITE);
    // Only update game logic once every wait_time.
    // Gets the snake to move 1 tile/s
    if (current_time - previous_time >= wait_time) {
      previous_time = GetTime();
      updateGame();
    }
    // Render the level
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        DrawRectangleRec(level[r][c].rect, level[r][c].colour);
      }
    }
    // Render all the entities
    for (int r = 0; r < ENTITY_TYPES; r++) {
      int count = entityArrLength(entities[r]);
      for (int c = 0; c < count; c++) {
        DrawRectangleRec(entities[r][c].rect, entities[r][c].colour);
      }
    }
    // Render the snake_body
    for (int s = 0; s < player.body_len; s++) {
      Entity segment = snake_body[s];
      DrawRectangleRec(segment.rect, segment.colour);
    }
    DrawRectangleRec(player.rect, GREEN); // Render the player
    displayUI(level_score, total_score); // This is always rendered last
    EndDrawing();
  }
  CloseWindow();
  free(snake_body);
}

