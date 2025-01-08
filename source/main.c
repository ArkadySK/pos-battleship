#include <stdio.h>
#include "board.h"
#include "menu.h"

int main(int argc, char const *argv[])
{
    int mode;
    board b_own;
    board b_enemy;
    
    mode = handle_menu();
    if (mode == 0) // Quit     
        return 0;

    board_init(&b_own, 10);
    board_init(&b_enemy, 10);
    board_display(&b_own, &b_enemy);
    
    clear_screen();
    if (mode == 1) { // Computer game mode
        printf("computer mode");       
        //TODO Adam: implement computer mode
    }
    if (mode == 2) { // Human vs Human (network) mode  
        printf("Please wait, connecting...\n");
        // TODO Adam: bind somehow with client.c :D  
    }

    board_destroy(&b_own);
    board_destroy(&b_enemy);
    return 0;
}
