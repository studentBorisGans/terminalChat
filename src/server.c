#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "network_utils.h" // For getting local IPs
#include "common.h"        // Shared constants and macros

// Function to handle a single client
void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg); // Free dynamically allocated memory for the socket descriptor
    char buffer[BUFF_SIZE];
    int bytes_received;

    printf("Client handler thread started.\n");

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        printf("Client says: %s\n", buffer);

        // Echo message back to the client
        if (send(client_socket, "Message received", 17, 0) < 0)
        {
            perror("Send failed");
            break;
        }

        // Check for "exit" command from the client
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Client requested to close the connection.\n");
            break;
        }
    }

    if (bytes_received == 0)
    {
        printf("Client disconnected.\n");
    }
    else if (bytes_received < 0)
    {
        perror("Receive failed");
    }

    close(client_socket);
    printf("Client handler thread exiting.\n");
    return NULL;
}

int main()
{
    int server_socket, *client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Display all available IP addresses
    printf("Available IP addresses:\n");
    get_local_ip();

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1)
    {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        // Allocate memory for client socket descriptor
        client_socket = malloc(sizeof(int));
        if (!client_socket)
        {
            perror("Memory allocation failed");
            continue;
        }

        // Accept a client connection
        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (*client_socket == -1)
        {
            perror("Client connection failed");
            free(client_socket);
            continue;
        }

        printf("New client connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a thread to handle the client
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, client_socket) != 0)
        {
            perror("Thread creation failed");
            close(*client_socket);
            free(client_socket);
        }
        else
        {
            pthread_detach(client_thread); // Automatically reclaim resources when the thread exits
        }
    }

    close(server_socket);
    return 0;
}
