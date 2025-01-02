#include <stdio.h>
#include <stdbool.h>

typedef struct board board;

struct board {
    char** board_;
    int destroyed_;
};

void board_init(board b, int size);

void place_ships(board b);

void board_display(board b, bool own);

int shoot(board b, int x, int y);