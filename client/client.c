#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../source/board.h"
#include "../source/menu.h"
#include "../source/utils.h"

#define PORT 8080
#define SERVER_IP "127.0.0.1" //localhost

int initialize_client() {
    int sock;
    struct sockaddr_in server_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("IC: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("IC: Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("IC: Connection failed");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void play_game(int sock) {
    board b_own;
    board b_enemy;
    
    // Initialize game boards
    board_init(&b_own, 10);
    board_init(&b_enemy, 10);
    
    // Display initial board state
    board_display(&b_own, &b_enemy);
    
    // Place ships
    place_ships(&b_own);
    
    // Game loop
    bool requestGameEnd = false;
    while (!requestGameEnd) {
        // TODO Adam: Implement game loop with server communication

    }
    
    // Cleanup
    board_destroy(&b_own);
    board_destroy(&b_enemy);
}

int main() {
    int mode;
    
    // Handle menu
    mode = handle_menu();
    if (mode == 0) // Quit     
        return 0;
        
    clear_screen();
    
    if (mode == 1) { // Computer game mode
        printf("Computer mode not yet implemented\n");
        return 0;
    }
    
    if (mode == 2) { // Human vs Human (network) mode
        printf("Please wait, connecting to server...\n");
        int sock = initialize_client();
        play_game(sock);
        close(sock);
    }

    return 0;
}
