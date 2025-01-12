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
#include "../source/message.h"

#define SERVER_IP "127.0.0.1" // localhost

int initialize_client(int port)
{
    int sock;
    struct sockaddr_in server_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("IC: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    //TODO Adam: do we need this?
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("IC: Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("IC: Connection failed");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void play_game(int sock)
{
    board b_own;
    board b_enemy;
    bool game_over = false;
    bool my_turn = false;
    Message msg;

    // Initialize game boards
    board_init(&b_own, 10);
    board_init(&b_enemy, 10);

    // Place ships
    place_ships(&b_own);

    // Game loop
    while (!game_over)
    {
        // receive message from server
        if (recv(sock, &msg, sizeof(Message), 0) <= 0) {
            perror("Play Game: Connection lost");
            break;
        }

        switch(msg.type) {
            case MSG_WAIT_PLAYER:
                show_message("Waiting for other player...");
                break;

            case MSG_START_GAME:
                clear_screen();
                show_message("Game started!");
                break;

            case MSG_YOUR_TURN:
                my_turn = true;
                break;

            case MSG_SHOT:
                // Received shot from opponent
                int hit =receive_shot(msg.x, msg.y, &b_own);

                if (hit == -1) {
                    printf("You lost!\n");
                    Message game_over_msg = {.type = MSG_GAME_OVER};
                    send(sock, &game_over_msg, sizeof(Message), 0);
                    game_over = true;
                    break;
                }
                
                // Send result back
                Message result = {
                    .type = MSG_RESULT,
                    .x = msg.x,
                    .y = msg.y,
                    .hit = hit
                };
                send(sock, &result, sizeof(Message), 0);
                break;

            case MSG_RESULT:
                // Result of our shot
                mark_hit(msg.x, msg.y, msg.hit, &b_enemy);
                break;

            case MSG_GAME_OVER:
                printf("You win!\n");
                game_over = true;
                break;
        }

        if (my_turn) {
            board_display(&b_own, &b_enemy);
            char* shot = shoot(&b_enemy);
            // TODO refactor?
            int x = shot[0] - 'A';
            int y = shot[1] - '0';
            free(shot);  // Add this to prevent memory leak

            // Send shot
            Message msg = {
                .type = MSG_SHOT,
                .x = x,
                .y = y
            };
            send(sock, &msg, sizeof(Message), 0);
            my_turn = false;
        }
    }

    // Cleanup
    board_destroy(&b_own);
    board_destroy(&b_enemy);
}

int main(int argc, char **argv)
{
    int mode;

    // Handle menu
    mode = handle_menu();
    if (mode == 0) // Quit
        return 0;

    clear_screen();

    if (mode == 1) // Computer game mode
    { 
        show_message("Computer mode not yet implemented");
        sleep(3);
        clear_screen();
        // TODO Waffle: implement
        return 0;
    }

    int port = 8536;
    if (argc >= 2)
        port = atoi(argv[1]);

    if (mode == 2) // Human vs Human (network) mode
    { 
        show_message("Please wait, connecting...");
        sleep(1);
        int sock = initialize_client(port);
        play_game(sock);
        close(sock);
    }

    return 0;
}
