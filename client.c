#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define PORT 8000
#define SERVER_IP "127.0.0.1"
#define BUFF_SIZE 1024


int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFF_SIZE];
    int bytes_received;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    // server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to Server at %s:%d\n", SERVER_IP, PORT);

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);  // Read message from user input

        // Send message to the server
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Message send failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        buffer[1024] = "";

        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("Receive failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';  // Null-terminate the response
        printf("Server response: %s\n", buffer);

        if (strcmp(buffer, "exit\n") == 0) {
            printf("Closing connection...\n");
            break;
        }
    }

    close(client_socket);
    return 0;
}