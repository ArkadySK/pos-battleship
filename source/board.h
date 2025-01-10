#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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

bool check_destroyed(int x, int y, board* b);

bool receive_shot(int x, int y, board* b);

void get_shot(char* shot, board* b_enemy);

char* shoot(board* b_enemy);

void mark_hit(char* shot, bool destroyed, board* b_enemy);