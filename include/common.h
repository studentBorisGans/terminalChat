#ifndef COMMON_H
#define COMMON_H

// Default port for the server and client to communicate
#define PORT 8000

// Buffer size for message transmission
#define BUFF_SIZE 1024
#define MAX_CHAT_ROOMS 5
// Macros for log levels (optional, for enhanced debugging)
#define LOG_INFO(msg) printf("[INFO]: %s\n", msg)
#define LOG_ERROR(msg) fprintf(stderr, "[ERROR]: %s\n", msg)

// Function declarations (if you need shared utility functions)
void log_message(const char *level, const char *message);

#endif // COMMON_H
