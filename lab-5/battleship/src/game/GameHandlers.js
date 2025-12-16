import { broadcast } from '../../ws_utils.js';
import { checkGameOver, validateShips } from './GameRules.js';
import { initializeBoard } from './GameStates.js';


// Обработка расстановки кораблей
export function handlePlaceShips(room, player, ships) {
  if (room.status !== 'placing') return;
  
  // Валидация кораблей
  if (!validateShips(ships)) {
    player.socket.send(JSON.stringify({ 
      type: 'error', 
      message: 'Invalid ship placement' 
    }));
    return;
  }
  
  // Сохранение кораблей и подготовка поля
  player.ships = ships;
  initializeBoard(player.board, ships);
  player.ready = true;
  
  broadcast(room.id, { 
    type: 'playerReady', 
    playerId: player.id,
    playerIndex: room.players.findIndex(p => p.id === player.id)
  });
  
  // Проверка готовности обоих игроков
  if (room.players.every(p => p.ready)) {
    room.status = 'playing';
    broadcast(room.id, { 
      type: 'startGame', 
      currentPlayer: room.players[room.currentPlayerIndex].id,
      currentPlayerIndex: room.currentPlayerIndex
    });
  }
}



// Обработка выстрела
export function handleFire(room, player, x, y) {
  if (room.status !== 'playing') return;
  
  // Проверка очереди хода
  if (room.players[room.currentPlayerIndex].id !== player.id) {
    player.socket.send(JSON.stringify({ 
      type: 'error', 
      message: 'Not your turn' 
    }));
    return;
  }
  
  // Проверка валидности координат
  if (x < 0 || x >= 10 || y < 0 || y >= 10) {
    player.socket.send(JSON.stringify({ 
      type: 'error', 
      message: 'Invalid coordinates' 
    }));
    return;
  }
  
  const meIndex = room.currentPlayerIndex;
  const me = room.players[meIndex];

  const opponentIndex = (room.currentPlayerIndex + 1) % 2;
  const opponent = room.players[opponentIndex];
  
  // Проверка, не стрелял ли уже сюда
  if (opponent.board[y][x] === 'hit' || opponent.board[y][x] === 'miss') {
    player.socket.send(JSON.stringify({ 
      type: 'error', 
      message: 'Already fired at this position' 
    }));
    return;
  }
  
  // Обработка выстрела
  let hit = false;
  if (opponent.board[y][x] === 'ship') {
    opponent.board[y][x] = 'hit';
    hit = true;
    room.lastHit = { x, y };
  } else {
    opponent.board[y][x] = 'miss';
    room.lastHit = null;
  }
  
  // Проверка на победу
  const gameOver = checkGameOver(opponent);
  
  // Определение следующего игрока
  if (!hit || gameOver) {
    room.currentPlayerIndex = opponentIndex;
  }
  
  // Рассылка результатов
  broadcast(room.id, {
    type: 'fireResult',
    x,
    y,
    hit,
    gameOver,
    winner: gameOver ? player.id : null,
    prevPlayer: room.players[meIndex].id,
    prevPlayerIndex: meIndex,
    currentPlayer: room.players[room.currentPlayerIndex].id,
    currentPlayerIndex: room.currentPlayerIndex
  });
  
  // Завершение игры при победе
  if (gameOver) {
    room.status = 'finished';
    broadcast(room.id, {
        type: 'gameOver',
        winner: player.id, // Текущий игрок - победитель
        winnerIndex: meIndex,
        reason: 'All opponent ships sunk'
    });
  }
}