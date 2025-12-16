// Инициализация игрового поля
export function initializeBoard(board, ships) {
  // Очистка поля
  for (let y = 0; y < 10; y++) {
    for (let x = 0; x < 10; x++) {
      board[y][x] = null;
    }
  }
  
  // Размещение кораблей
  for (const ship of ships) {
    for (let i = 0; i < ship.size; i++) {
      let x = ship.x, y = ship.y;
      if (ship.direction === 'horizontal') x += i;
      else y += i;
      
      if (x >= 0 && x < 10 && y >= 0 && y < 10) {
        board[y][x] = 'ship';
      }
    }
  }
}