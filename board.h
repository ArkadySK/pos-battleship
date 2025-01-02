#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define NOT_HIT '.'
#define HIT_WATER 'o'
#define HIT_SHIP 'X'
#define SHIP '█'

typedef struct board board;

struct board {
    char** board_;
    int size_;
    int destroyed_;
};

void board_init(board* b, int size);

void place_ships(board* b);

void board_display(board* b_own, board* b_enemy);

int shoot(board* b, int x, int y);