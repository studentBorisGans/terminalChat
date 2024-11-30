
# Terminal Chat Application

This project is a simple terminal-based chat application that allows multiple clients to communicate with a server over a TCP connection. It is lightweight and designed for ease of use.

---

## **Features**
- **Multi-client support:** Multiple clients can connect to the server simultaneously.
- **Dynamic IP configuration:** Clients can connect to any server by providing its IP address.
- **Simple command-line interface:** Both server and client are easy to use.

---

## **Setup Instructions**

### **1. Prerequisites**
Before running the application, ensure the following:
1. **C Compiler**:
   - **Linux/macOS:** `gcc` or `clang`
   - **Windows:** Install MinGW or use WSL.
2. **CMake**: Install CMake for building the project:
   - Download it from [CMake.org](https://cmake.org/download/).
3. **Network**:
   - Both the server and client must be on the same network.

---

### **2. Clone the Repository**
Clone the repository to your local machine:
```bash
git clone https://github.com/yourusername/TerminalChat.git
cd TerminalChat
```

---

### **3. Build the Project**
1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Run CMake to configure the build:
   ```bash
   cmake ..
   ```

3. Build the executables:
   ```bash
   cmake --build .
   ```

4. Verify the executables:
   - Navigate to the `bin` directory:
     ```bash
     cd bin
     ls
     ```
   - You should see:
     ```plaintext
     Client Server
     ```

---

## **Running the Application**

### **1. Start the Server**
1. Navigate to the `bin` directory:
   ```bash
   cd build/bin
   ```

2. Start the server:
   ```bash
   ./Server
   ```

3. The server will display available local IP addresses and start listening on port `8000`. Example output:
   ```plaintext
   Local IP addresses:
   Interface: en0        Address: 192.168.1.100
   Server is listening on port 8000...
   ```

4. Share the server's IP address (e.g., `192.168.1.100`) with anyone who wants to connect.

---

### **2. Connect Using the Client**
1. On another machine or terminal, navigate to the `bin` directory:
   ```bash
   cd build/bin
   ```

2. Run the client and provide the server's IP address:
   ```bash
   ./Client <server_ip>
   ```

3. Example:
   ```bash
   ./Client 192.168.1.100
   ```

4. You’ll see:
   ```plaintext
   Resolved server address: 192.168.1.100
   Connected to server at 192.168.1.100:8000
   Enter message (type 'exit' to quit):
   ```

5. Type messages to send them to the server. The server will log the messages, and you’ll receive responses.

---

### **3. Disconnect the Client**
1. Type `exit` to disconnect from the server:
   - **Client Output:**
     ```plaintext
     Closing connection...
     ```
   - **Server Output:**
     ```plaintext
     Client requested to close the connection.
     Client handler thread exiting.
     ```

2. The server remains active and can accept new connections.

---

## **Testing the Application**
1. Open multiple terminal windows or machines.
2. Run the server in one terminal and connect multiple clients using their respective IP addresses.
3. Verify that messages from all clients are being handled properly by the server.

---

## **Troubleshooting**

### **1. Cannot Connect to Server**
- Ensure the server is running and its IP address is correct.
- Confirm that port `8000` is open:
  - **Linux/macOS:** Check with `lsof -i :8000`.
  - **Windows:** Use Task Manager or `netstat -a`.
- If the firewall is blocking connections:
  - **macOS:** Temporarily disable the firewall:
    ```bash
    sudo /usr/libexec/ApplicationFirewall/socketfilterfw --setglobalstate off
    ```
  - **Linux:** Allow port `8000` using `ufw`:
    ```bash
    sudo ufw allow 8000/tcp
    ```
  - **Windows:** Allow TCP connections on port `8000` through the firewall.

### **2. Client Does Not Receive a Response**
- Check the server logs for errors.
- Ensure the server is not overloaded and is actively processing connections.

### **3. Ping Test Fails**
- Ensure both server and client are on the same network.
- Use `ping <server_ip>` to test connectivity.

---
