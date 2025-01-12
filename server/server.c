#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include "../source/message.h"

#define MAX_PLAYERS 2

atomic_int player_count = 0; // Atomic variable to track connected players

typedef struct {
    int socket;
    int opponent_socket;
    bool has_opponent;
} Player;

Player players[MAX_PLAYERS];
pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;

int find_player_slot(int socket) {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(players[i].socket == socket) {
            return i;
        }
    }
    return -1;
}

int find_empty_slot() {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(players[i].socket == 0) {
            return i;
        }
    }
    return -1;
}

int initialize_server(int port)
{
    int server_fd;
    struct sockaddr_in address;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("IS: Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("IS: Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0)
    {
        perror("IS: Listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void *handle_client(void *arg)
{
    //Dereference the pointer to get the socket
    int client_socket = *(int *)arg;
    
    // Register player
    pthread_mutex_lock(&players_mutex);
    int player_slot = find_empty_slot();
    if(player_slot == -1) {
        pthread_mutex_unlock(&players_mutex);
        close(client_socket);
        return NULL;
    }
    
    players[player_slot].socket = client_socket;
    players[player_slot].has_opponent = false;
    
    //Try to find opponent, assign opponent socket and set has_opponent to true
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(i != player_slot && players[i].socket != 0 && !players[i].has_opponent) {
            players[player_slot].opponent_socket = players[i].socket;
            players[player_slot].has_opponent = true;
            players[i].opponent_socket = client_socket;
            players[i].has_opponent = true;
            break;
        }
    }
    pthread_mutex_unlock(&players_mutex);
    
    // Wait for opponent
    while(!players[player_slot].has_opponent) {
        Message msg = {.type = MSG_WAIT_PLAYER};
        send(client_socket, &msg, sizeof(Message), 0);
        sleep(1);
        
        pthread_mutex_lock(&players_mutex);
        if(atomic_load(&player_count) < 2) {
            pthread_mutex_unlock(&players_mutex);
            continue;
        }
        pthread_mutex_unlock(&players_mutex);
    }

    // Start game
    Message start_msg = {.type = MSG_START_GAME};
    send(client_socket, &start_msg, sizeof(Message), 0);
    
    // Wait a moment to ensure both players have received START_GAME
    usleep(100000);  // 100ms
    
    // First player gets first turn, but only after both are ready
    if(player_slot == 0) {
        Message turn_msg = {.type = MSG_YOUR_TURN};
        send(client_socket, &turn_msg, sizeof(Message), 0);
    } else {
        Message wait_msg = {.type = MSG_WAIT_PLAYER};
        send(client_socket, &wait_msg, sizeof(Message), 0);
    }

    // Game loop
    while(true) {
        Message msg;
        if(recv(client_socket, &msg, sizeof(Message), 0) <= 0) {
            break;
        }

        pthread_mutex_lock(&players_mutex);
        int opponent_socket = players[player_slot].opponent_socket;
        pthread_mutex_unlock(&players_mutex);

        switch(msg.type) {
            case MSG_SHOT:
                Message wait_msg = {.type = MSG_WAIT_PLAYER};
                send(client_socket, &wait_msg, sizeof(Message), 0);

                // Forward shot to opponent
                send(opponent_socket, &msg, sizeof(Message), 0);
                break;
                
            case MSG_RESULT:
                // Forward result to shooter
                send(opponent_socket, &msg, sizeof(Message), 0);
                // Send turn message to the player who just got shot at
                Message turn_msg = {.type = MSG_YOUR_TURN};
                send(client_socket, &turn_msg, sizeof(Message), 0);
                break;
            
            case MSG_GAME_OVER:
                send(opponent_socket, &msg, sizeof(Message), 0);
                break;
        }
    }

    pthread_mutex_lock(&players_mutex);
    int opponent_socket = players[player_slot].opponent_socket;
    
    // TODO Adam: check if player receives game over message
    // Send game over to opponent if they're still connected
    if(players[player_slot].has_opponent) {
        Message game_over = {.type = MSG_GAME_OVER};
        send(opponent_socket, &game_over, sizeof(Message), 0);
    }
    
    // Destroy player slot
    players[player_slot].socket = 0;
    players[player_slot].opponent_socket = 0;
    players[player_slot].has_opponent = false;
    
    // destroy opponent's data
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if(players[i].socket == opponent_socket) {
            players[i].opponent_socket = 0;
            players[i].has_opponent = false;
            break;
        }
    }
    pthread_mutex_unlock(&players_mutex);

    atomic_fetch_sub(&player_count, 1);
    close(client_socket);
    return NULL;
}

int main(int argc, char** argv)
{
    // Initialize players array with 0s
    memset(players, 0, sizeof(players));
    
    int port = 8536;
    if(argc >= 2)
        port = atoi(argv[1]);

    int server_fd = initialize_server(port);

    printf("Server is running on port %d\n", port);

    while (true)
    {
        printf("player_count: %d\n", atomic_load(&player_count));
        if (atomic_load(&player_count) < MAX_PLAYERS)
        {
            struct sockaddr_in address;
            int addrlen = sizeof(address);
            int *new_socket = malloc(sizeof(int)); // Allocate memory for the socket

            int accepted_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
            *new_socket = accepted_socket;

            if (accepted_socket < 0)
            {
                perror("Server: Accept failed");
                free(new_socket); // Free memory in case of failure
                continue;
            }

            // Increment the player count atomically
            atomic_fetch_add(&player_count, 1);

            // Create a thread to handle the client
            pthread_t thread_id;
            if (pthread_create(&thread_id, NULL, handle_client, new_socket) != 0)
            {
                perror("Server: Thread creation failed");
                atomic_fetch_sub(&player_count, 1);
                close(*new_socket);
                free(new_socket);
            }
            else
            {
                // Launch the thread but dont wait for it to finish
                pthread_detach(thread_id); 
            }
        }
        else
        {
            //printf("Server: Maximum players connected. Connection rejected.\n");
            sleep(1); // Optional sleep to avoid busy waiting
        }
    }

    close(server_fd);
    return 0;
}
