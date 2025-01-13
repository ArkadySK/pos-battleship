#include "client.h"

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

    // Convert server ip to binary
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
            char* shot = calloc(3, sizeof(char));
            shoot(shot, &b_enemy);
            int x;
            int y;
            parse_input(shot, &x, &y, NULL);
            free(shot);

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
    int port = DEFAULT_PORT;
    if (argc >= 2)
        port = atoi(argv[1]);

    // Handle menu
    mode = handle_menu();
    if (mode == 0) // Quit
        return 0;

    clear_screen();

    if (mode == 1) // Computer singleplayer game mode
    { 
        show_message("Connecting to computer opponent...");
        sleep(1);
        int sock = initialize_client(port);
        
        Message mode_msg = {.type = MSG_SINGLE_PLAYER};
        send(sock, &mode_msg, sizeof(Message), 0);
        
        play_game(sock);
        close(sock);
        return 0;
    }

    if (mode == 2) // Human vs Human multiplayer mode
    { 
        show_message("Please wait, connecting...");
        sleep(1);
        int sock = initialize_client(port);
        
        Message mode_msg = {.type = MSG_MULTI_PLAYER};
        send(sock, &mode_msg, sizeof(Message), 0);
        
        play_game(sock);
        close(sock);
    }

    return 0;
}
