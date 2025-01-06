#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//IPC: Inter-Process Communication

#define PORT 8080

// Function to initialize server socket
int initialize_server_socket() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("ISS: Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("ISS: Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("ISS: Listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

// Function to initialize client socket
int initialize_client_socket() {
    int sock;
    struct sockaddr_in server_addr;
    const char *localhost_ip_address = "127.0.0.1";  // localhost address

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ICS: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, localhost_ip_address, &server_addr.sin_addr) <= 0) {
        perror("ICS: Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ICS: Connection failed");
        exit(EXIT_FAILURE);
    }

    return sock;
}
