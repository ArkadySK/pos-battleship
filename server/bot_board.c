#include "bot_board.h"

void generate_board(board* b)
{
    int placed = 0;
    int size = 5;
    char* position = calloc(3, sizeof(char));
    while(placed < 5)
    {
        position[0] = 'A' + rand() % b->size_;
        position[1] = '0' + rand() % b->size_;
        position[2] = rand() % 2 ? 'd' : 'r';
        if (validate_position(position, size, b))
        {
            finalise_placement(position, size, b);
            placed++;
            if (placed != 3)
            {
                size--;
            }
        }
    }
    free(position);
}

void generate_shot(char* shot, board* b_enemy)
{
    while (true)
    {
        shot[0] = 'A' + rand() % b_enemy->size_;
        shot[1] = '0' + rand() % b_enemy->size_;
        shot[2] = '\0';
        int x;
        int y;
        parse_input(shot, &x, &y, NULL);
        if (b_enemy->board_[x][y] != NOT_HIT)
        {
            continue;
        }
        break;
    }
}