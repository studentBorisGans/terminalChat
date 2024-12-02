import React, { useState } from 'react';
import Login from './components/Login';
import ChatRoom from './components/ChatRoom';
import MessageList from './components/MessageList';
import MessageInput from './components/MessageInput';
import './styles/App.css';

const App = () => {
  const [username, setUsername] = useState(null);
  const [currentRoom, setCurrentRoom] = useState(null);

  const handleLogin = (user) => {
    console.log('User logged in:', user); // Debug log
    setUsername(user);
  };

  const handleRoomSelect = (room) => {
    console.log('Room selected:', room); // Debug log
    setCurrentRoom(room.name);
  };

  return (
    <div className="App">
      {!username ? (
        <Login onLogin={handleLogin} />
      ) : !currentRoom ? (
        <ChatRoom onRoomSelect={handleRoomSelect} username={username}/>
      ) : (
        <div className="Chat">
<MessageList roomName={currentRoom} username={username} />
          <MessageInput roomName={currentRoom} username={username} />
  </div>
      )}
    </div>
  );
};

export default App;
