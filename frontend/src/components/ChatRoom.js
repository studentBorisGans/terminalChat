import React, { useState, useEffect } from 'react';
import socket from '../socket'; // Ensure you're importing the socket instance
import '../styles/ChatRoom.css';
import api from '../api';

function ChatRoom({ onRoomSelect, username }) { // Pass `username` dynamically
  const [rooms, setRooms] = useState([]);
  const [error, setError] = useState(null);
  useEffect(() => {
    // Emit the username when the component is mounted
    if (username) {
      socket.emit('setUsername', username);
    }

  }, [username]);
  // Fetch the chat rooms from the backend on component mount
  useEffect(() => {
    const fetchRooms = async () => {
      try {
        const response = await api.get('/rooms'); // Fetch rooms from the backend
        setRooms(response.data); // Assuming the response contains an array of rooms
      } catch (err) {
        console.error('Error fetching chat rooms:', err);
        setError('Failed to load chat rooms.');
      }
    };
    
    fetchRooms();
  }, []);

  // Handle room selection and emit 'joinRoom' to backend via socket
  const handleRoomClick = (room) => {
    if (username) {
      socket.emit('joinRoom', { roomName: room.name, username }); // Emit room join with dynamic username
      onRoomSelect(room); // Pass selected room to the parent component
    } else {
      setError('Username is required to join a room.'); // Error handling if no username is provided
    }
  };

  return (
    <div className="ChatRoom">
      <h2>Available Chat Rooms</h2>
      {error && <div className="error">{error}</div>} {/* Show error message if fetching fails */}
      <ul>
        {rooms.length > 0 ? (
          rooms.map((room, index) => (
            <li key={index} onClick={() => handleRoomClick(room)}>
              {room.name} ({room.isPrivate ? 'Private' : 'Public'})
            </li>
          ))
        ) : (
          <div>No rooms available</div>
        )}
      </ul>
    </div>
  );
}

export default ChatRoom;
