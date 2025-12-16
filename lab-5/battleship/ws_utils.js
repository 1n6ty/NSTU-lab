import { rooms } from "./settings.js";

// Функция рассылки сообщений всем игрокам в комнате
export function broadcast(roomId, message) {
  const room = rooms[roomId];
  if (!room) return;
  
  room.players.forEach(player => {
    if (player.socket.readyState === WebSocket.OPEN) {
      try {
        player.socket.send(JSON.stringify(message));
      } catch (error) {
        console.error('Error sending message:', error);
      }
    }
  });
}
