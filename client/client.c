#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

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

void communicate(int sock) {
    char *message = "Hello from client!";
    char buffer[1024] = {0};

    // Send message to server
    send(sock, message, strlen(message), 0);
    printf("Client: Message sent to server: %s\n", message);

    // Receive response from server
    read(sock, buffer, 1024);
    printf("Client: Message from server: %s\n", buffer);
}

int main() {
    int sock = initialize_client();
    communicate(sock);
    close(sock);
    return 0;
}
