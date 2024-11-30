#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include "network_utils.h" // For getting local IPs
#include "common.h"        // Shared constants and macros

#define MAX_CHAT_ROOMS 5
#define MAX_USERS_PER_ROOM 10

typedef struct ChatRoom
{
    char name[50];
    int is_private;
    char password[50];
    int user_count;
    int client_sockets[MAX_USERS_PER_ROOM]; // Sockets for broadcasting
    char users[MAX_USERS_PER_ROOM][50];     // List of usernames
} ChatRoom;

// Global array of chat rooms
ChatRoom chat_rooms[MAX_CHAT_ROOMS] = {
    {"General", 0, "", 0},      // Public room
    {"Devs Only", 1, "1234", 0} // Private room
};

// Mutex for thread-safe updates to chat rooms
pthread_mutex_t room_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to get the current timestamp
void get_timestamp(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, size, "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec);
}

// Function to display chat rooms to the client
void send_chat_rooms(int client_socket)
{
    char room_list[BUFF_SIZE] = "Available Chat Rooms:\n";
    strcat(room_list, "---------------------------------\n");
    for (int i = 0; i < MAX_CHAT_ROOMS; i++)
    {
        if (strlen(chat_rooms[i].name) > 0)
        {
            char room_info[100];
            snprintf(room_info, sizeof(room_info), "%d. %s (%s) - %d users\n", i + 1, chat_rooms[i].name,
                     chat_rooms[i].is_private ? "Private" : "Public", chat_rooms[i].user_count);
            strcat(room_list, room_info);
        }
    }
    strcat(room_list, "---------------------------------\n");
    send(client_socket, room_list, strlen(room_list), 0);
}

// Function to add a user to a chat room
int add_user_to_room(ChatRoom *room, int client_socket, const char *username)
{
    pthread_mutex_lock(&room_mutex);
    if (room->user_count >= MAX_USERS_PER_ROOM)
    {
        pthread_mutex_unlock(&room_mutex);
        return -1; // Room is full
    }
    room->client_sockets[room->user_count] = client_socket;
    strcpy(room->users[room->user_count], username);
    room->user_count++;
    pthread_mutex_unlock(&room_mutex);
    return 0;
}

// Function to remove a user from a chat room
void remove_user_from_room(ChatRoom *room, int client_socket)
{
    pthread_mutex_lock(&room_mutex);
    for (int i = 0; i < room->user_count; i++)
    {
        if (room->client_sockets[i] == client_socket)
        {
            for (int j = i; j < room->user_count - 1; j++)
            {
                room->client_sockets[j] = room->client_sockets[j + 1];
                strcpy(room->users[j], room->users[j + 1]);
            }
            room->user_count--;
            break;
        }
    }
    pthread_mutex_unlock(&room_mutex);
}

// Function to broadcast messages to all clients in a room
void broadcast_message(ChatRoom *room, int sender_socket, const char *message)
{
    pthread_mutex_lock(&room_mutex);
    for (int i = 0; i < room->user_count; i++)
    {
        int client_socket = room->client_sockets[i];
        if (client_socket != sender_socket) // Skip the sender
        {
            send(client_socket, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&room_mutex);
}

// Function to list users in a room
void list_users_in_room(ChatRoom *room, char *buffer)
{
    pthread_mutex_lock(&room_mutex);
    strcpy(buffer, "Active users:\n");
    for (int i = 0; i < room->user_count; i++)
    {
        strcat(buffer, room->users[i]);
        strcat(buffer, "\n");
    }
    pthread_mutex_unlock(&room_mutex);
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

        if (strncmp(buffer, "/", 1) == 0) // Handle commands
        {
            if (strcmp(buffer, "/help") == 0)
            {
                const char *help_message = "Commands:\n/help - Show this menu\n/exit - Leave the room\n/users - List active users\n";
                send(client_socket, help_message, strlen(help_message), 0);
            }
            else if (strcmp(buffer, "/users") == 0)
            {
                char user_list[BUFF_SIZE];
                list_users_in_room(room, user_list);
                send(client_socket, user_list, strlen(user_list), 0);
            }
            else if (strcmp(buffer, "/exit") == 0)
            {
                snprintf(join_message, sizeof(join_message), "%s has left the chat.\n", username);
                broadcast_message(room, -1, join_message);
                break;
            }
            else
            {
                send(client_socket, "Unknown command\n", 16, 0);
            }
            continue;
        }

        // Broadcast message to the room
        char chat_message[BUFF_SIZE];
        snprintf(chat_message, sizeof(chat_message), "[%s]: %s\n", username, buffer);
        broadcast_message(room, client_socket, chat_message);
    }

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
