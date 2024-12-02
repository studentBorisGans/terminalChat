import React, { useState } from 'react';
import socket from '../socket'; // Import the socket instance
import '../styles/MessageInput.css';

function MessageInput({ roomName, username }) {
  const [message, setMessage] = useState('');
  const [error, setError] = useState(null);

  // Validate message before sending
  const isValidMessage = () => {
    if (!roomName) {
      setError('Room name is required to send a message.');
      return false;
    }
    if (!username) {
      setError('Username is required to send a message.');
      return false;
    }
    if (message.trim() === '') {
      setError('Message cannot be empty.');
      return false;
    }
    setError(null); // Clear any existing errors
    return true;
  };

  // Handle sending the message
  const handleSend = () => {
    if (isValidMessage()) {
      const messagePayload = { message, roomName, username };
      console.log('Sending message:', messagePayload);
      socket.emit('message', messagePayload, (ack) => {
        // Optional: Handle acknowledgment from the server
        if (ack?.status !== 'ok') {
          console.error('Message failed to send:', ack?.error || 'Unknown error');
        }
      });
      setMessage(''); // Clear the input field
    }
  };

  // Handle input changes
  const handleChange = (e) => {
    setMessage(e.target.value);
  };

  // Handle "Enter" key press
  const handleKeyPress = (e) => {
    if (e.key === 'Enter') {
      handleSend(); // Trigger send on Enter
    }
  };

  return (
    <div className="MessageInput">
      <input
        type="text"
        value={message}
        onChange={handleChange}
        onKeyPress={handleKeyPress}
        placeholder="Type a message..."
        className={error ? 'error' : ''}
        aria-label="Message Input"
      />
      <button onClick={handleSend} disabled={!message.trim()}>
        Send
      </button>

      {error && <div className="error-message">{error}</div>}
    </div>
  );
}

export default MessageInput;
