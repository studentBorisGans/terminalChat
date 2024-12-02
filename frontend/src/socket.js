import { io } from 'socket.io-client';

// Use your machine's actual IP address or a public URL accessible by the frontend
const SOCKET_URL = 'http://172.16.2.102:8001'; // Add the protocol (http:// or https://) and port

const socket = io(SOCKET_URL, {
  transports: ['websocket'], // Specify transports
  withCredentials: true,      // Ensure that credentials are allowed if required
});

socket.on('connect', () => {
  console.log('Socket connected with ID:', socket.id);
});

socket.on('connect_error', (error) => {
  console.error('Socket connection error:', error);
});

export default socket;
