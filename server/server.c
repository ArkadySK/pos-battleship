#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int initialize_server() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("IS: Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binds the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("IS: Bind failed");
        exit(EXIT_FAILURE);
    }

    // Server starts listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("IS: Listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};

    // Read message from client
    read(client_socket, buffer, 1024);
    printf("HC: Message from client: %s\n", buffer);

    // Send response to client
    send(client_socket, "Hello from server", strlen("Hello from server"), 0);
    printf("HC: Message sent to client\n");

    close(client_socket);
}

int main() {
    int server_fd = initialize_server();
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Accept connection from client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Server: Accept failed");
        exit(EXIT_FAILURE);
    }

    handle_client(new_socket);

    close(server_fd);
    return 0;
}
