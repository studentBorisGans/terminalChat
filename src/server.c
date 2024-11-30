#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#include "network_utils.h" // For getting local IPs
#include "common.h"        // Shared constants and macros
#include "chat_room.h"     // Include the ChatRoom definition

#define MAX_CHAT_ROOMS 5
#define MAX_USERS_PER_ROOM 10

typedef struct User
{
    char username[50];
    int socket;
    struct User *next;
} User;

// Global linked list of users
User *user_list = NULL;
pthread_mutex_t user_list_mutex = PTHREAD_MUTEX_INITIALIZER;

// Mutex for thread-safe updates to chat rooms
pthread_mutex_t room_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to get the current timestamp
void get_timestamp(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, size, "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec);
}

// Function to add a user to the user list
void add_user(const char *username, int socket)
{
    pthread_mutex_lock(&user_list_mutex);
    User *new_user = (User *)malloc(sizeof(User));
    strncpy(new_user->username, username, sizeof(new_user->username) - 1);
    new_user->username[sizeof(new_user->username) - 1] = '\0';
    new_user->socket = socket;
    new_user->next = user_list;
    user_list = new_user;
    pthread_mutex_unlock(&user_list_mutex);
}

// Function to remove a user from the user list
void remove_user(int socket)
{
    pthread_mutex_lock(&user_list_mutex);
    User *prev = NULL, *curr = user_list;

    while (curr != NULL)
    {
        if (curr->socket == socket)
        {
            if (prev == NULL)
            {
                user_list = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    pthread_mutex_unlock(&user_list_mutex);
}

// Function to find a user by username
User *find_user(const char *username)
{
    pthread_mutex_lock(&user_list_mutex);
    User *curr = user_list;

    while (curr != NULL)
    {
        if (strcmp(curr->username, username) == 0)
        {
            pthread_mutex_unlock(&user_list_mutex);
            return curr;
        }
        curr = curr->next;
    }

    pthread_mutex_unlock(&user_list_mutex);
    return NULL;
}

// Function to send a direct message
void handle_dm(int sender_socket, const char *sender_username, const char *target_username, const char *message)
{
    User *target_user = find_user(target_username);

    if (target_user != NULL)
    {
        char dm_message[BUFF_SIZE];
        snprintf(dm_message, sizeof(dm_message), "[DM from %s]: %s\n", sender_username, message);
        send(target_user->socket, dm_message, strlen(dm_message), 0);
    }
    else
    {
        const char *error_message = "User not found.\n";
        send(sender_socket, error_message, strlen(error_message), 0);
    }
}

// Function to handle commands
void handle_command(ChatRoom *room, int client_socket, const char *username, const char *command)
{
    char buffer[BUFF_SIZE];

    if (strncmp(command, "/dm ", 4) == 0)
    {
        char target_username[50];
        const char *message = strchr(command + 4, ' ');

        if (message != NULL)
        {
            strncpy(target_username, command + 4, message - (command + 4));
            target_username[message - (command + 4)] = '\0';
            message++; // Skip the space

            handle_dm(client_socket, username, target_username, message);
        }
        else
        {
            const char *error_message = "Invalid DM format. Use /dm <username> <message>\n";
            send(client_socket, error_message, strlen(error_message), 0);
        }
    }
    else if (strcmp(command, "/help") == 0)
    {
        const char *help_message = "Commands:\n/help - Show this menu\n/exit - Leave the room\n/users - List active users\n/dm <username> <message> - Send a private message\n";
        send(client_socket, help_message, strlen(help_message), 0);
    }
    else if (strcmp(command, "/users") == 0)
    {
        char user_list[BUFF_SIZE];
        list_users_in_room(room, user_list);
        send(client_socket, user_list, strlen(user_list), 0);
    }
    else if (strcmp(command, "/exit") == 0)
    {
        snprintf(buffer, sizeof(buffer), "%s has left the chat.\n", username);
        broadcast_message(room, client_socket, buffer);
        remove_user_from_room(room, client_socket);
        close(client_socket);
        pthread_exit(NULL);
    }
    else
    {
        const char *error_message = "Unknown command\n";
        send(client_socket, error_message, strlen(error_message), 0);
    }
}

// Function to handle a single client
void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    free(arg);

    char buffer[BUFF_SIZE];
    char username[50];
    int bytes_received;

    // Receive username
    bytes_received = recv(client_socket, username, sizeof(username) - 1, 0);
    if (bytes_received <= 0)
    {
        close(client_socket);
        return NULL;
    }
    username[bytes_received] = '\0'; // Null-terminate username
    add_user(username, client_socket);

    printf("User connected: %s\n", username);

    // Send chat rooms to client
    send_chat_rooms(client_socket);

    // Receive room selection
    int room_number;
    bytes_received = recv(client_socket, &room_number, sizeof(room_number), 0);
    if (bytes_received <= 0 || room_number < 1 || room_number > MAX_CHAT_ROOMS)
    {
        close(client_socket);
        return NULL;
    }

    ChatRoom *room = &chat_rooms[room_number - 1];

    // Handle private room authentication
    if (room->is_private)
    {
        send(client_socket, "Enter password: ", 17, 0);
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0 || strcmp(buffer, room->password) != 0)
        {
            send(client_socket, "Access Denied\n", 14, 0);
            close(client_socket);
            return NULL;
        }
    }

    // Add user to room
    if (add_user_to_room(room, client_socket, username) != 0)
    {
        send(client_socket, "Room is full. Please try another room.\n", 40, 0);
        close(client_socket);
        return NULL;
    }

    printf("User %s joined room: %s\n", username, room->name);
    char join_message[BUFF_SIZE];
    snprintf(join_message, sizeof(join_message), "%s has joined the chat.\n", username);
    broadcast_message(room, -1, join_message);

    // Chat loop
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate message

        if (buffer[0] == '/') // Command
        {
            handle_command(room, client_socket, username, buffer);
        }
        else
        {
            char chat_message[BUFF_SIZE];
            snprintf(chat_message, sizeof(chat_message), "[%s]: %s\n", username, buffer);
            broadcast_message(room, client_socket, chat_message);
        }
    }

    remove_user(client_socket);
    remove_user_from_room(room, client_socket);
    close(client_socket);
    printf("User %s disconnected from room: %s\n", username, room->name);
    return NULL;
}

int main()
{
    int server_socket, *client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

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
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces

    // Bind and listen
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 10) == -1)
    {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        client_socket = malloc(sizeof(int));
        if (!client_socket)
        {
            perror("Memory allocation failed");
            continue;
        }

        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (*client_socket == -1)
        {
            perror("Client connection failed");
            free(client_socket);
            continue;
        }

        printf("New client connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, client_socket) != 0)
        {
            perror("Thread creation failed");
            close(*client_socket);
            free(client_socket);
        }
        else
        {
            pthread_detach(client_thread);
        }
    }

    close(server_socket);
    return 0;
}
