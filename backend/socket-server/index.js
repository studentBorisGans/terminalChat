const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const cors = require('cors');
const userMap = new Map(); // Maps socket.id to username

const app = express();
const server = http.createServer(app);

const io = new Server(server, {
  cors: {
    origin: "http://172.16.2.102:3000", // Only allow your frontend to connect
    methods: ["GET", "POST"],
    allowedHeaders: ["Content-Type"],
    credentials: true, // If you're handling cookies or credentials
  },
  transports: ['websocket', 'polling'], // Enable both WebSocket and polling transports
});

// Middleware
app.use(cors());
app.use(express.json());

// Store messages in memory (for simplicity)
let messages = [];
let rooms = [
  { name: 'General', isPrivate: false },
  { name: 'Devs Only', isPrivate: true },
];

// Test route to verify server is running
app.get('/', (req, res) => {
  res.send('Hello from the server!');
});

// Route to get the available rooms
app.get('/rooms', (req, res) => {
  res.json(rooms); // Return rooms as JSON response
});

// WebSocket events
io.on('connection', (socket) => {
  console.log('A user connected:', socket.id);

  // Test listener
  socket.on('test', (data) => {
    console.log('Test event received:', data);
  });

  // Assign username to the connected socket
  socket.on('setUsername', (username) => {
    userMap.set(socket.id, username);
    console.log(`User ${socket.id} set username: ${username}`);
  });

  // Handle joining a room
  socket.on('joinRoom', (room) => {
    socket.join(room);
    console.log(`User ${socket.id} joined room: ${room}`);
  });

  // Handle incoming messages
  socket.on('message', (msg) => {
    const username = userMap.get(socket.id) || 'Anonymous';
    const messageWithUser = { ...msg, username };
    console.log('Broadcasting message:', messageWithUser); // Debug log
    io.emit('message', messageWithUser); // Broadcast to all connected clients
  });

  // Handle disconnection
  socket.on('disconnect', () => {
    const username = userMap.get(socket.id);
    console.log(`User disconnected: ${socket.id} (Username: ${username})`);
    userMap.delete(socket.id); // Remove user from map
  });
});

// Start the server
const PORT = 8001;
server.listen(PORT, '0.0.0.0', () => {
  console.log(`Socket.IO server running on http://0.0.0.0:${PORT}`);
});
