# TerminalChat - A Command-Line Chat Application

## Overview
TerminalChat is a multi-client, server-based chat application that allows users to communicate in chat rooms and send private direct messages (DMs). Designed for simplicity and functionality, TerminalChat leverages socket programming and multithreading to support multiple clients seamlessly.

## Features
- **Chat Rooms**: Public and private chat rooms.
- **Direct Messaging (DMs)**: Send private messages to specific users.
- **Real-time Messaging**: Broadcast messages to all users in a chat room.
- **Command Support**: Helpful commands to enhance the chat experience.
- **Secure Private Rooms**: Password-protected rooms for private discussions.
- **User Management**: Dynamic handling of active users in rooms.

---

## How to Use

### Prerequisites
- CMake (version 3.10 or higher)
- GCC/Clang (with C11 support)
- A terminal emulator
- Multiple terminal windows to simulate client connections

---

### Setup and Build
1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd terminalChat
   ```

2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```

3. Configure the project:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build .
   ```

5. The compiled executables will be available in the `bin/` directory.

---

### Running the Application
#### Start the Server
In a terminal, navigate to the `bin/` directory and run:
```bash
./Server
```
The server listens for incoming client connections.

#### Start a Client
In another terminal, navigate to the `bin/` directory and run:
```bash
./Client <server-ip>
```
Replace `<server-ip>` with the IP address or hostname of the server (e.g., `127.0.0.1` for localhost).

---

## Commands

### Available Commands
- **`/help`**: Display a list of available commands.
- **`/dm <username> <message>`**: Send a direct message to a specific user.
- **`/users`**: List all active users in the current room.
- **`/exit`**: Leave the chat room and disconnect.

---

## Testing the Application
1. Start the server in one terminal.
2. Open multiple terminals to simulate clients.
3. Connect clients to the server and:
   - Join a chat room.
   - Broadcast messages to the room.
   - Send private messages using `/dm`.

---

## File Structure
```
terminalChat/
├── src/
│   ├── client.c        # Client implementation
│   ├── server.c        # Server implementation
│   ├── chat_room.c     # Chat room utilities
│   ├── network_utils.c # Network utilities
├── include/
│   ├── chat_room.h     # Chat room header
│   ├── common.h        # Shared constants and declarations
│   ├── network_utils.h # Network-related utilities
├── CMakeLists.txt      # CMake configuration
```

---

## Example Usage

### Join a Room
1. Start the server.
2. Start a client, provide your username, and join a room:
   ```
   Enter your username: Alice
   Connected to server as Alice.
   Available Chat Rooms:
   1. General (Public) - 0 users
   2. Devs Only (Private) - 0 users
   Select a chat room: 1
   Welcome to the chat room!
   ```

### Broadcast a Message
Type a message and press `Enter`:
```
> Hello, everyone!
```

### Send a Direct Message
Use the `/dm` command:
```
> /dm Bob Hi Bob! How are you?
```

### Leave the Chat Room
Use the `/exit` command:
```
> /exit
```

