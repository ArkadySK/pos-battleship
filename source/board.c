#include "board.h"

void board_init(board* b, int size)
{
    b->board_ = malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++)
    {
        b->board_[i] = calloc(size, sizeof(char));
        for (int j = 0; j < size; j++)
        {
            b->board_[i][j] = NOT_HIT;
        } 
    }

    b->size_ = size;
    b->destroyed_ = 0;
}

void board_display(board* b_own, board* b_enemy)
{
    printf("       Your board      |      Enemy board\n");
    printf("   A B C D E F G H I J |   A B C D E F G H I J\n");
    for (int i = 0; i < b_own->size_; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < b_own->size_; j++)
        {
            printf("%c ", b_own->board_[i][j]);
        }
        printf("|%2d ", i + 1);
        for (int j = 0; j < b_enemy->size_; j++)
        {
            if (b_enemy->board_[i][j] = SHIP)
            {
                printf("%c ", NOT_HIT);
            }
            else
            {
                printf("%c ", b_enemy->board_[i][j]);
            }
        }
        printf("\n");
    }
}

void place_ships(board* b)
{
    printf("Ship coordinates are given as: \"2Ad\" ");
    printf("where 2A are the coordinates of the ship, ");
    printf("and the last letter is either \"d\" or \"r\" for down or right\n");
    printf("Where do you want to place the 5 tile ship?");
    // TODO: Waffle
}

int shoot(board* b, int x, int y)
{
    // TODO: Waffle
}