#include "custom_structs.h"
#include "handy_funcs.h"

int entityArrLength(Entity arr[]) {
  int count = 0;
  while (arr[count].type != '\0') {
    count++;
  }
  return count;
}

