#include "board.h"

int main(int argc, char const *argv[])
{
    board b_own;
    board_init(&b_own, 10);
    board b_enemy;
    board_init(&b_enemy, 10);
    board_display(&b_own, &b_enemy);
    return 0;
}
