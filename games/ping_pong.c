#include <stdio.h>

#define MAX_COLS 80
#define MAX_ROWS 25

void map_drawing();

int main() {
  map_drawing();

  return 0;
}

void map_drawing() {
  for (int x = 0; x <= MAX_ROWS; x++) {
    for (int y = 0; y <= MAX_COLS; y++) {
      if (x == 0 || x == MAX_ROWS) {
        printf("—");
      } else if (y == 0 || y == (MAX_COLS / 2) || y == MAX_COLS) {
        printf("|");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}
