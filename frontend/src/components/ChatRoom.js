import React, { useState, useEffect } from 'react';
import api from '../api';
import socket from '../socket';
import MessageList from './MessageList';
import MessageInput from './MessageInput';
import '../styles/ChatRoom.css';

function ChatRoom({ onRoomSelect }) {
  const [rooms, setRooms] = useState([]);
  const [error, setError] = useState(null);
  const [password, setPassword] = useState('');
  const [roomToJoin, setRoomToJoin] = useState(null); // Store selected room
  const [joinedRoom, setJoinedRoom] = useState(null); // Track the joined room
  const [isRoomJoined, setIsRoomJoined] = useState(false); // New state to track room join status

  useEffect(() => {
    // Fetch chat rooms from the backend
    api
      .get('/rooms')
      .then((response) => {
        setRooms(response.data); // Assuming the API returns an array of rooms
      })
      .catch((error) => {
        console.error('Error fetching chat rooms:', error);
        setError('Failed to load chat rooms.');
      });
  }, []);

  const handleRoomClick = (room) => {
    console.log('Room clicked:', room); // Debugging log to check which room is selected
    if (room.isPrivate) {
      setRoomToJoin(room); // Show password prompt for private rooms
    } else {
      // Join public rooms directly
      socket.emit('joinRoom', { roomName: room.name });
      console.log('Joining room:', room.name); // Debugging log to check joining room
      setJoinedRoom(room); // Set the joined room in state
      setIsRoomJoined(true); // Mark room as joined
      onRoomSelect(room); // Notify parent component (if needed)
    }
  };

  const handlePasswordSubmit = (e) => {
    e.preventDefault();
    console.log('Trying to join room:', roomToJoin.name, 'with password:', password); // Debug log for password submission

    // Emit joinRoom event with password for private rooms
    socket.emit('joinRoom', { roomName: roomToJoin.name, password });
    
    // Wait for confirmation before joining
    socket.on('roomJoined', () => {
      console.log('Room joined successfully'); // Debugging log for successful room join
      setJoinedRoom(roomToJoin); // Update joined room state
      setIsRoomJoined(true); // Mark room as joined
      setRoomToJoin(null); // Hide password prompt
      setPassword(''); // Reset password field
    });

    // Handle incorrect password
    socket.on('error', (message) => {
      console.error('Password error:', message); // Debugging log for password error
      alert(message); // Show error message (you can style this better)
      setPassword(''); // Reset password field on failure
    });
  };

  return (
    <div className="ChatRoom">
      {!isRoomJoined ? (
        <>
          <h2>Available Chat Rooms</h2>
          {error && <div className="error">{error}</div>}
          <ul>
            {rooms.map((room, index) => (
              <li key={index} onClick={() => handleRoomClick(room)}>
                {room.name} ({room.isPrivate ? 'Private' : 'Public'})
              </li>
            ))}
          </ul>
        </>
      ) : (
        // If the room is joined, show the chat interface
        <div className="Chat">
          <h3>{joinedRoom.name}</h3>
          {/* Include the message list and input */}
          <MessageList roomName={joinedRoom.name} />
          <MessageInput roomName={joinedRoom.name} username="User1" />
        </div>
      )}

      {/* If the room is private, show the password input */}
      {roomToJoin && !isRoomJoined && (
        <div className="passwordPrompt">
          <h3>Enter password for {roomToJoin.name}</h3>
          <input
            type="password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            placeholder="Password"
          />
          <button onClick={handlePasswordSubmit}>Join Room</button>
        </div>
      )}
    </div>
  );
}

export default ChatRoom;
