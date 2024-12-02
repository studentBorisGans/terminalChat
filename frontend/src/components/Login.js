import React, { useState } from 'react';
import socket from '../socket'; // Ensure you are importing the configured socket
import '../styles/Login.css';

function Login({ onLogin }) {
  const [username, setUsername] = useState('');

  const handleLogin = () => {
    if (username.trim() !== '') {
      socket.emit('setUsername', username); // Send username to backend
      onLogin(username); // Pass username to App component
    } else {
      alert('Username cannot be empty.');
    }
  };

  return (
    <div className="Login">
      <h2>Welcome to TerminalTalk</h2>
      <input
        type="text"
        placeholder="Enter your username"
        value={username}
        onChange={(e) => setUsername(e.target.value)}
      />
      <button onClick={handleLogin}>Login</button>
    </div>
  );
}

export default Login;
