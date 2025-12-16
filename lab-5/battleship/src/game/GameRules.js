// Валидация кораблей
export function validateShips(ships) {
  const board = Array(10).fill().map(() => Array(10).fill(false));
  const requiredShips = { 1: 4, 2: 3, 3: 2, 4: 1 };
  const shipCount = { 1: 0, 2: 0, 3: 0, 4: 0 };
  
  for (const ship of ships) {
    if (!requiredShips[ship.size] || shipCount[ship.size] >= requiredShips[ship.size]) {
      return false;
    }
    
    shipCount[ship.size]++;

    for (let i = 0; i < ship.size; i++) {
      let x = ship.x, y = ship.y;
      if (ship.direction === 'horizontal') x += i;
      else y += i;
      
      // Проверка границ поля
      if (x < 0 || x >= 10 || y < 0 || y >= 10) return false;
      
      // Проверка на пересечение с другими кораблями
      if (board[y][x]){
        return false;
      }
      
      // Проверка соседних клеток (правило одного пробела)

      for (let dy = -1; dy <= 1; dy++) {
        for (let dx = -1; dx <= 1; dx++) {
          const nx = x + dx, ny = y + dy;
          if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
            if (board[ny][nx]){
                return false;
            }
          }
        }
      }
      
    }

    for (let i = 0; i < ship.size; i++) {
        board[ship.y][ship.x] = true;
    }
  }
  
  // Проверка количества кораблей
  for (const size in requiredShips) {
    if (shipCount[size] !== requiredShips[size]) return false;
  }
  
  return true;
}



// Проверка на окончание игры
export function checkGameOver(player) {
  for (let y = 0; y < 10; y++) {
    for (let x = 0; x < 10; x++) {
      if (player.board[y][x] === 'ship') {
        return false;
      }
    }
  }
  return true;
}
