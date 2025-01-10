#include "utils.h"

// Clears the screen using ANSI escape sequence
void clear_screen() {
    // \033 - represents the ASCII escape character
    // \033[H - Moves the cursor to the "home" position (top-left corner of the terminal)
    printf("\033[2J\033[H");    
}