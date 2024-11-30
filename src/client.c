#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network_utils.h" // For hostname resolution
#include "common.h"        // Shared constants and macros

int main(int argc, char *argv[])
{
    // Ensure server hostname or IP is provided as an argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <server_hostname_or_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_input = argv[1];
    char server_ip[INET_ADDRSTRLEN]; // To store resolved IP address

    // Resolve hostname to IP if needed
    if (resolve_hostname(server_input, server_ip, sizeof(server_ip)) != 0)
    {
        fprintf(stderr, "Failed to resolve hostname or invalid IP: %s\n", server_input);
        exit(EXIT_FAILURE);
    }

    printf("Resolved server address: %s\n", server_ip);

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFF_SIZE];
    int bytes_received;

    // Create a TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid server IP address");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection to server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", server_ip, PORT);

    // Chat loop: Send and receive messages
    while (1)
    {
        printf("Enter message (type 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline character from input
        buffer[strcspn(buffer, "\n")] = '\0';

        // Send message to the server
        if (send(client_socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("Message send failed");
            break;
        }

        // Exit on client-side "exit" command
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Closing connection...\n");
            break;
        }

        // Receive response from the server
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            perror("Receive failed or server closed the connection");
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the response
        printf("Server response: %s\n", buffer);

        // Exit if the server sends "exit"
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Server closed the connection.\n");
            break;
        }
    }

    close(client_socket);
    return 0;
}
