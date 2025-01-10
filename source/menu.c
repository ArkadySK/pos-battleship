#include "menu.h"

char display_menu(void) {
    clear_screen();

    printf("\n");
    printf("*********************************\n");
    printf("*         BATTLESHIP           *\n");
    printf("*********************************\n");
    printf("\n");
    printf("Welcome! Choose an activity:\n");
    printf("\n");
    printf("[1] Play against Computer\n");
    printf("[2] Play against Human\n");
    printf("[q] Quit\n");
    printf("\n");
    printf("Your choice: ");

    // Read single character
    char choice = getchar();
    while (getchar() != '\n');

    return choice;
}

int handle_menu(void) {
    char choice;
    int running = 1;

    while (running) {
        choice = display_menu();
        switch (choice) {
            case '1':
                return 1;
            case '2':
                return 2;
            case 'q':
            case 'Q':
                return 0;   
        }
    }
    return 0;
}

void show_message(char* message) {
    clear_screen();
    printf("  -------------------------------  \n  ");
    printf(message);
    printf("  -------------------------------  \n");
}
