#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <termios.h>       // For masking password input
#include "network_utils.h" // For hostname resolution
#include "common.h"        // Shared constants and macros

// Global client socket
int client_socket;

// Function to mask password input
void get_password(char *password, size_t size)
{
    printf("Enter password: ");
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fgets(password, size, stdin);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");

    password[strcspn(password, "\n")] = '\0'; // Remove newline
}

// Function to display chat rooms
void display_chat_rooms(const char *room_list)
{
    printf("\n%s", room_list);
}

// Function to validate user input
int get_valid_input(int min, int max)
{
    int choice;
    while (1)
    {
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1 || choice < min || choice > max)
        {
            printf("Invalid input. Please select a valid option.\n");
            while (getchar() != '\n')
                ; // Clear the input buffer
        }
        else
        {
            getchar(); // Consume newline character
            return choice;
        }
    }
}

// Function to clear the current line
void clear_current_line()
{
    printf("\033[2K\r"); // ANSI escape code to clear line
}

// Thread function to receive messages from the server
void *receive_messages(void *arg)
{
    char buffer[BUFF_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';

        // Save the current line, display the message, and restore the prompt
        clear_current_line();
        printf("%s\n", buffer); // Display received message
        printf("> ");           // Redraw the input prompt
        fflush(stdout);
    }

    if (bytes_received == 0)
    {
        printf("Server closed the connection.\n");
    }
    else
    {
        perror("Receive failed");
    }

    close(client_socket);
    exit(0);
}

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

    struct sockaddr_in server_addr;
    char buffer[BUFF_SIZE];

    // Prompt user for a username
    char username[50];
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove newline character

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
    printf("Connecting to server...\n");
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection to server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send username to the server
    send(client_socket, username, strlen(username), 0);

    printf("Connected to server as %s.\n", username);

    // Display chat rooms
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        display_chat_rooms(buffer);
    }

    // Prompt for chat room selection
    printf("Select a chat room: ");
    int room_number = get_valid_input(1, MAX_CHAT_ROOMS);
    send(client_socket, &room_number, sizeof(room_number), 0);

    // Check if the room is private
    if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) > 0 && strcmp(buffer, "Enter password: ") == 0)
    {
        char password[50];
        get_password(password, sizeof(password));
        send(client_socket, password, strlen(password), 0);

        // Check password validation
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        buffer[bytes_received] = '\0'; // Null-terminate
        if (strcmp(buffer, "Access Denied\n") == 0)
        {
            printf("Access denied. Disconnecting...\n");
            close(client_socket);
            return 0;
        }
    }

    printf("Welcome to the chat room!\n");

    // Create a thread for receiving messages
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0)
    {
        perror("Failed to create receive thread");
        close(client_socket);
        return 1;
    }

    // Main chat loop for sending messages
    while (1)
    {
        printf("> "); // Prompt for user input
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline character from input
        buffer[strcspn(buffer, "\n")] = '\0';

        // Check for commands
        if (strncmp(buffer, "/", 1) == 0)
        {
            if (strcmp(buffer, "/help") == 0)
            {
                printf("Commands:\n/help - Show this menu\n/exit - Leave the room\n/dm <username> <message> - Send a private message\n");
                continue;
            }
            else if (strncmp(buffer, "/dm ", 4) == 0)
            {
                // Send DM command to the server
                if (send(client_socket, buffer, strlen(buffer), 0) < 0)
                {
                    perror("Failed to send DM command");
                }
                continue;
            }
            else if (strcmp(buffer, "/exit") == 0)
            {
                printf("Closing connection...\n");
                break;
            }
            else
            {
                printf("Unknown command: %s\n", buffer);
                continue;
            }
        }

        // Send regular message to the server
        if (send(client_socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("Message send failed");
            break;
        }
    }

    pthread_cancel(receive_thread);
    pthread_join(receive_thread, NULL);
    close(client_socket);
    return 0;
}
