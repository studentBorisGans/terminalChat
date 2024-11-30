// chat_room.h
#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#define MAX_USERS_PER_ROOM 10 // Maximum number of users allowed in a chat room
#define MAX_CHAT_ROOMS 5      // Maximum number of chat rooms

// Structure representing a chat room
typedef struct ChatRoom
{
    char name[50];                          // Name of the chat room
    int is_private;                         // 1 if the room is private, 0 otherwise
    char password[50];                      // Password for private rooms
    int user_count;                         // Current number of users in the room
    int client_sockets[MAX_USERS_PER_ROOM]; // Sockets of connected clients
    char users[MAX_USERS_PER_ROOM][50];     // Usernames of connected clients
} ChatRoom;

// Global chat rooms array
extern ChatRoom chat_rooms[MAX_CHAT_ROOMS];

#endif // CHAT_ROOM_H
