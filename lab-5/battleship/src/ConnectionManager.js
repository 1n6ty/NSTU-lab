import { v4 as uuidv4 } from 'uuid';
import { rooms } from '../settings.js';
import { broadcast } from '../ws_utils.js';
import { handlePlayerDisconnect, handleWebSocketMessage } from './WebSocketHandlers.js';

export const connectionManager = (socket, req) => {
  const url = new URL(req.url, `http://${req.headers.host}`);
  const roomId = url.searchParams.get('roomId');
  const playerId = uuidv4();
  
  // Проверка наличия ID комнаты
  if (!roomId) {
    socket.send(JSON.stringify({ type: 'error', message: 'Room ID is required' }));
    socket.close();
    return;
  }

  // Создание новой комнаты или подключение к существующей
  if (!rooms[roomId]) {
    rooms[roomId] = {
      id: roomId,
      players: [],
      status: 'waiting', // 'placing', 'playing', 'finished'
      currentPlayerIndex: 0,
      lastHit: null
    };
  }

  const room = rooms[roomId];

  // Проверка на заполненность комнаты
  if (room.players.length >= 2) {
    socket.send(JSON.stringify({ type: 'error', message: 'Room is full' }));
    socket.close();
    return;
  }

  // Создание нового игрока
  const player = {
    id: playerId,
    socket: socket,
    ships: [],
    board: Array(10).fill().map(() => Array(10).fill(null)),
    ready: false,
    isReadyToPlay: false
  };

  room.players.push(player);
  
  // Отправка информации о подключении
  socket.send(JSON.stringify({ 
    type: 'joined', 
    playerId: playerId, 
    roomId: roomId,
    playerIndex: room.players.length - 1
  }));
  
  // Отправка информации другим игрокам в комнате
  if (room.players.length > 1) {
    broadcast(roomId, {
      type: 'playerJoined',
      playerId: playerId,
      playerIndex: room.players.length - 1
    });
  }

  // Начало фазы расстановки кораблей при подключении второго игрока
  if (room.players.length === 2 && room.status === 'waiting') {
    room.status = 'placing';
    broadcast(roomId, { type: 'startPlacing' });
  }

  // Обработка сообщений от клиента
  socket.on('message', (data) => {
    try {
      const message = JSON.parse(data);
      handleWebSocketMessage(roomId, playerId, message);
    } catch (error) {
      console.error('Error parsing message:', error);
      socket.send(JSON.stringify({ type: 'error', message: 'Invalid message format' }));
    }
  });

  // Обработка отключения клиента
  socket.on('close', () => {
    handlePlayerDisconnect(roomId, playerId);
  });
};
