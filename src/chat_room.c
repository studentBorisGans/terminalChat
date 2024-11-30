#include "chat_room.h"
#include <sys/socket.h> // For send
#include <string.h>     // For strlen
#include <stdio.h>      // Optional for debugging
#include "common.h"

void send_chat_rooms(int client_socket)
{
    char room_list[BUFF_SIZE] = "Available Chat Rooms:\n";
    strcat(room_list, "---------------------------------\n");
    for (int i = 0; i < MAX_CHAT_ROOMS; i++)
    {
        if (strlen(chat_rooms[i].name) > 0) // Skip empty rooms
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

int add_user_to_room(ChatRoom *room, int client_socket, const char *username)
{
    if (room->user_count >= MAX_USERS_PER_ROOM)
    {
        return -1; // Room is full
    }
    room->client_sockets[room->user_count] = client_socket;
    strncpy(room->users[room->user_count], username, sizeof(room->users[room->user_count]));
    room->user_count++;
    return 0;
}

void remove_user_from_room(ChatRoom *room, int client_socket)
{
    for (int i = 0; i < room->user_count; i++)
    {
        if (room->client_sockets[i] == client_socket)
        {
            for (int j = i; j < room->user_count - 1; j++)
            {
                room->client_sockets[j] = room->client_sockets[j + 1];
                strncpy(room->users[j], room->users[j + 1], sizeof(room->users[j]));
            }
            room->user_count--;
            break;
        }
    }
}

void broadcast_message(ChatRoom *room, int sender_socket, const char *message)
{
    for (int i = 0; i < room->user_count; i++)
    {
        int client_socket = room->client_sockets[i];
        if (client_socket != sender_socket) // Skip the sender
        {
            send(client_socket, message, strlen(message), 0);
        }
    }
}

void list_users_in_room(ChatRoom *room, char *buffer)
{
    strcpy(buffer, "Active users:\n");
    for (int i = 0; i < room->user_count; i++)
    {
        strcat(buffer, room->users[i]);
        strcat(buffer, "\n");
    }
}
// Define the global chat_rooms array
ChatRoom chat_rooms[MAX_CHAT_ROOMS] = {
    {"General", 0, "", 0},      // Public room
    {"Devs Only", 1, "1234", 0} // Private room
};