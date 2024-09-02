#include "../include/level_loader.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

FILE *fptr;

typedef struct Entity {
  Rectangle rect;
} Entity;

void generateLevel(const char* file_name) {
  char level[32][32];
  int line_count = 0;

  fptr = fopen(file_name, "r");
  if (fptr == NULL) {
    perror("\nUnable open file\n");
    exit(-1);
  }
//  char buffer[100];
//  while (!feof(fptr)) {
//    fread(buffer, sizeof(buffer), 1, fptr);
//    line_count++;
//  }
//  if (line_count > 32) {
//    perror("Incorrect number of lines. 32 > line counts > 0\n");
//    exit(-1);
//  }
  char buffertwo[200];
  while (fgets(buffertwo, 200, fptr)) {
    printf("%s", buffertwo);
  }
  fclose(fptr);
}

int main() {
  generateLevel("../levels/level1.txt");
}

