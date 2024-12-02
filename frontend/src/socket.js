import { io } from 'socket.io-client';


const SOCKET_URL = 'http://172.16.2.102:8001'; // backend server URL

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
