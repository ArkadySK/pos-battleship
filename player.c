#include "player.h"

void board_init(board* b, int size)
{
    b->board_ = malloc(size, sizeof(char*));
    for (int i = 0; i < size; i++)
    {
        b->board_[i] = calloc(size, sizeof(char));
        for (int j = 0; j < size; j++)
        {
            b->board_[i][j] = '.';
        } 
    }

    b->destroyed_ = 0;
}

void board_display(board b, bool own)
{
    printf("   A B C D E F G H I J\n");
    // TODO: Waffle
}

void place_ships(board b)
{
    board_display(b, true);
    printf("Where do you want to place the 5 tile ship?");
    // TODO: Waffle
}

void shoot(board b)
{
    // TODO: Waffle
}