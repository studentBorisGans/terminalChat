import React from 'react';
import ReactDOM from 'react-dom';
import App from './App';
import './index.css';

import socket from './socket';

// Debug WebSocket connection on load
socket.on('connect', () => {
  console.log('Socket connected:', socket.id);
});
socket.on('message', (msg) => {
  console.log('Received message:', msg); // Debug messages globally
});

ReactDOM.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
  document.getElementById('root')
);
