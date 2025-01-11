#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../source/board.h"
#include "../source/menu.h"
#include "../source/utils.h"

#define SERVER_IP "127.0.0.1" //localhost

int initialize_client(int port) {
    int sock;
    struct sockaddr_in server_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("IC: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

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
    
    // Place ships
    place_ships(&b_own);
    
    // Game loop
    bool requestGameEnd = false;
    while (!requestGameEnd) {
        // TODO Adam: Implement game loop with server communication
        // Display initial board state
        // board_display(&b_own, &b_enemy);
    }
    
    // Cleanup
    board_destroy(&b_own);
    board_destroy(&b_enemy);
}

int main(int argc, char** argv) {
    int mode;
    
    // Handle menu
    mode = handle_menu();
    if (mode == 0) // Quit     
        return 0;
        
    clear_screen();
    
    if (mode == 1) { // Computer game mode
        show_message("Computer mode not yet implemented");
        sleep(3);
        clear_screen();
        //TODO Waffle: implement
        return 0;
    }
    
    int port = 8536;
    if(argc >= 2)
        port = atoi(argv[1]);

    if (mode == 2) { // Human vs Human (network) mode
        show_message("Please wait, connecting...");
        sleep(1);
        int sock = initialize_client(port);
        play_game(sock);
        close(sock);
    }

    return 0;
}
