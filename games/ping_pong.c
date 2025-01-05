#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// Размеры игрового поля
#define FIELD_WIDTH 80
#define FIELD_HEIGHT 20

// Параметры ракеток
#define RACKET_LENGTH 3
#define RACKET_MIN_X 1
#define RACKET_MAX_X (FIELD_HEIGHT - RACKET_LENGTH)

// Параметры игры
#define MAXIMUM_POINTS 11

// Функции для управления игрой
void process_input(int *left_racket_x, int *right_racket_x);
void move_racket(int *racket_x, int direction, int min_y, int max_y);

int is_goal(int ball_y);
void reset_ball(int *ball_x, int *ball_y, int *ball_dy);
int is_left_racket_collision(int ball_x, int ball_y, int left_racket_x);
int is_right_racket_collision(int ball_x, int ball_y, int right_racket_x);
void ball_logic(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy,
                int *score_p1, int *score_p2, int left_racket_x,
                int right_racket_x);

void draw_game_field(int left_racket_x, int right_racket_x, int ball_x,
                     int ball_y, int score_p1, int score_p2);
char get_racket_character(int row, int col, int left_racket_x,
                          int right_racket_x);

// Функции управления режимом терминала
void enable_raw_mode();
void disable_raw_mode();

struct termios original_termios;

int main() {
  int left_racket_x = 5, right_racket_x = 8;
  int ball_x = FIELD_HEIGHT / 2, ball_y = FIELD_WIDTH / 2;
  int ball_dx = 1, ball_dy = 1;
  int score_p1 = 0, score_p2 = 0;

  enable_raw_mode();

  while (score_p1 < MAXIMUM_POINTS && score_p2 < MAXIMUM_POINTS) {
    draw_game_field(left_racket_x, right_racket_x, ball_x, ball_y, score_p1,
                    score_p2);

    process_input(&left_racket_x, &right_racket_x);

    ball_logic(&ball_x, &ball_y, &ball_dx, &ball_dy, &score_p1, &score_p2,
               left_racket_x, right_racket_x);
  }

  disable_raw_mode();
  return 0;
}

int is_goal(int ball_y) { return ball_y == 0 || ball_y == FIELD_WIDTH; }

void reset_ball(int *ball_x, int *ball_y, int *ball_dy) {
  *ball_x = FIELD_HEIGHT / 2;
  *ball_y = FIELD_WIDTH / 2;
  *ball_dy *= -1;
}

int is_left_racket_collision(int ball_x, int ball_y, int left_racket_x) {
  return ball_y == 2 && ball_x >= left_racket_x &&
         ball_x < left_racket_x + RACKET_LENGTH;
}

int is_right_racket_collision(int ball_x, int ball_y, int right_racket_x) {
  return ball_y == FIELD_WIDTH - 2 && ball_x >= right_racket_x &&
         ball_x < right_racket_x + RACKET_LENGTH;
}

void ball_logic(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy,
                int *score_p1, int *score_p2, int left_racket_x,
                int right_racket_x) {
  *ball_x += *ball_dx;
  *ball_y += *ball_dy;

  // Отражение от стен
  if (*ball_x == 1 || *ball_x == FIELD_HEIGHT - 1) {
    *ball_dx *= -1;
  }

  // Проверка на гол
  if (is_goal(*ball_y)) {
    if (*ball_y == 0) {
      (*score_p2)++;
    } else if (*ball_y == FIELD_WIDTH) {
      (*score_p1)++;
    }
    reset_ball(ball_x, ball_y, ball_dy);
  }

  // Отражение от ракеток
  if (is_left_racket_collision(*ball_x, *ball_y, left_racket_x) ||
      is_right_racket_collision(*ball_x, *ball_y, right_racket_x)) {
    *ball_dy *= -1;
  }
}

char get_racket_character(int row, int col, int left_racket_x,
                          int right_racket_x) {
  if (col == 1 && row >= left_racket_x && row < left_racket_x + RACKET_LENGTH) {
    return '[';
  } else if (col == FIELD_WIDTH - 1 && row >= right_racket_x &&
             row < right_racket_x + RACKET_LENGTH) {
    return ']';
  }
  return ' ';
}

void draw_game_field(int left_racket_x, int right_racket_x, int ball_x,
                     int ball_y, int score_p1, int score_p2) {
  printf("\033c");

  for (int row = 0; row <= FIELD_HEIGHT; row++) {
    for (int col = 0; col <= FIELD_WIDTH; col++) {
      if (row == ball_x && col == ball_y) {
        printf("*");
      } else if (col == FIELD_WIDTH / 2) {
        printf("|");
      } else if (row == 0 || row == FIELD_HEIGHT) {
        if (col == 36) {
          printf("%02d", score_p1);
          col++;
        } else if (col == 43) {
          printf("%02d", score_p2);
          col++;
        } else {
          printf("—");
        }
      } else if (col == 0 || col == FIELD_WIDTH) {
        printf("|");
      } else {
        printf("%c",
               get_racket_character(row, col, left_racket_x, right_racket_x));
      }
    }
    printf("\n");
  }
  // Отладка
  printf("left_racket_x: %d\n", left_racket_x);
  printf("right_racket_x: %d\n", right_racket_x);
  printf("ball_x: %d\n", ball_x);
  printf("ball_y: %d\n", ball_y);
}

void move_racket(int *racket_x, int direction, int min_y, int max_y) {
  if ((*racket_x + direction >= min_y) && (*racket_x + direction <= max_y)) {
    *racket_x += direction;
  }
}

void process_input(int *left_racket_x, int *right_racket_x) {
  char input_char = getchar();

  switch (input_char) {
    case 'a':
    case 'A':
      move_racket(left_racket_x, -1, RACKET_MIN_X, RACKET_MAX_X);
      break;
    case 'z':
    case 'Z':
      move_racket(left_racket_x, 1, RACKET_MIN_X, RACKET_MAX_X);
      break;
    case 'k':
    case 'K':
      move_racket(right_racket_x, -1, RACKET_MIN_X, RACKET_MAX_X);
      break;
    case 'm':
    case 'M':
      move_racket(right_racket_x, 1, RACKET_MIN_X, RACKET_MAX_X);
      break;
  }
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &original_termios);
  struct termios raw = original_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}
