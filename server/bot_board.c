#include "board.h"

void generate_board(board* b)
{
    int placed = 0;
    int size = 5;
    char* position = malloc(3 * sizeof(char));
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
}