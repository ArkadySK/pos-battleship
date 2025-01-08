#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NOT_HIT '.'
#define HIT_WATER 'o'
#define HIT_SHIP 'X'
#define SHIP '#'
#define DOWN 'd'
#define RIGHT 'r'

typedef struct board board;

struct board {
    char** board_;
    int size_;
    int destroyed_;
};

void board_init(board* b, int size);

void board_destroy(board* b);

void board_display(board* b_own, board* b_enemy);

bool validate_coords(char* coords);

bool validate_rotation(char rotation);

bool validate_position(char* position, int size, board* b);

void get_ship(char* position, int size, board* b);

void finalise_placement(char* position, int size, board* b);

void place_ships(board* b);

int shoot(board* b, int x, int y);