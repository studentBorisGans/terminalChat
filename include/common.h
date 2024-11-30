#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h> // For send
#include "chat_room.h"  // Include ChatRoom structure and related constants

// Default port for the server and client
#define PORT 8000

// Buffer size for message transmission
#define BUFF_SIZE 1024

// Function declarations
void send_chat_rooms(int client_socket);
int add_user_to_room(ChatRoom *room, int client_socket, const char *username);
void remove_user_from_room(ChatRoom *room, int client_socket);
void broadcast_message(ChatRoom *room, int sender_socket, const char *message);
void list_users_in_room(ChatRoom *room, char *buffer);

#endif // COMMON_H
