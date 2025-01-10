#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

#define PORT 8536
#define MAX_PLAYERS 2

atomic_int player_count = 0; // Atomic variable to track connected players

int initialize_server()
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("IS: Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

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
    int client_socket = *(int *)arg;
    free(arg); // Free memory allocated for client_socket

    char buffer[1024] = {0};

    // Read message from client
    read(client_socket, buffer, 1024);
    printf("HC: Message from client: %s\n", buffer);

    // Send response to client
    const char* message = "Hello from server";
    send(client_socket, message, strlen(message), 0);
    printf("HC: Message sent to client\n");

    // Decrement the player count atomically
    atomic_fetch_sub(&player_count, 1);

    close(client_socket);
    return NULL;
}

int main()
{
    int server_fd = initialize_server();

    printf("Server is running on port %d\n", PORT);

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
                atomic_fetch_sub(&player_count, 1); // Decrement count if thread creation fails
                close(*new_socket);
                free(new_socket);
            }
            else
            {
                pthread_detach(thread_id); // Detach the thread to avoid memory leaks
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
