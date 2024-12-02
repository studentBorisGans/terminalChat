import React, { useState, useEffect, useRef } from 'react';
import socket from '../socket';
import '../styles/MessageList.css';

function MessageList({ roomName, username }) { // Ensure `username` is passed as a prop
  const [messages, setMessages] = useState([]);
  const messagesEndRef = useRef(null);

  const scrollToBottom = () => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  };

  useEffect(() => {
    const handleMessage = (message) => {
      console.log('Received message:', message);
      if (message.roomName === roomName) {
        setMessages((prevMessages) => [...prevMessages, message]);
      }
    };

    socket.on('message', handleMessage);

    return () => {
      socket.off('message', handleMessage); // Clean up listener on unmount
    };
  }, [roomName]);

  useEffect(() => {
    scrollToBottom();
  }, [messages]);

  return (
    <div className="MessageList">
      {messages.length === 0 && <div className="noMessages">No messages yet</div>}
      {messages.map((message, index) => (
        <div
          key={index}
          className={`MessageItem ${message.username === username ? 'currentUser' : ''}`}
        >
          <span className="sender">{message.username || 'Unknown User'}</span>
          <span className="text">{message.message || ''}</span>
        </div>
      ))}
      <div ref={messagesEndRef} />
    </div>
  );
}

export default MessageList;
