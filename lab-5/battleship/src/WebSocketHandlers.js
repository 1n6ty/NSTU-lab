import { rooms } from '../settings.js';
import { handleFire, handlePlaceShips } from './game/GameHandlers.js';
import { broadcast } from '../ws_utils.js';

// Обработка сообщений WebSocket
export function handleWebSocketMessage(roomId, playerId, message) {
  const room = rooms[roomId];
  if (!room) return;

  const player = room.players.find(p => p.id === playerId);
  if (!player) return;

  switch (message.type) {
    case 'placeShips':
      handlePlaceShips(room, player, message.ships);
      break;
      
    case 'fire':
      handleFire(room, player, message.x, message.y);
      break;
      
    case 'chatMessage':
      broadcast(roomId, {
        type: 'chatMessage',
        playerId: playerId,
        message: message.text,
        playerIndex: room.players.findIndex(p => p.id === playerId)
      });
      break;
  }
}



// Обработка отключения игрока
export function handlePlayerDisconnect(roomId, playerId) {
  const room = rooms[roomId];
  if (!room) return;
  
  const playerIndex = room.players.findIndex(p => p.id === playerId);
  if (playerIndex === -1) return;
  
  // Если игра еще не началась, просто удаляем игрока
  if (room.status === 'waiting' || room.status === 'placing') {
    room.players.splice(playerIndex, 1);
    
    if (room.players.length === 0) {
      delete rooms[roomId];
      return;
    }
    
    broadcast(roomId, {
      type: 'playerDisconnected',
      playerId: playerId,
      message: 'Player disconnected'
    });
    
    return;
  }
  
  // Если игра началась, объявляем победу другого игрока
  if (room.status === 'playing' || room.status === 'finished') {
    const winnerIndex = (playerIndex + 1) % 2;
    const winner = room.players[winnerIndex];
    
    broadcast(roomId, {
      type: 'gameOver',
      winner: winner.id,
      winnerIndex: winnerIndex,
      reason: 'Opponent disconnected'
    });
    
    room.status = 'finished';
  }
}
