// public/script.js
class SeaBattleGame {
  constructor() {
    this.setDefault();
    
    this.setupEventListeners();
    this.checkForRoomId();
  }
  
  setDefault() {
    this.socket = null;
    this.roomId = null;
    this.playerId = null;
    this.playerIndex = null;
    this.gameState = 'start'; // start, waiting, placing, playing, finished
    this.shipsToPlace = [
      { size: 4, count: 1, placed: 0, direction: 'horizontal' },
      { size: 3, count: 2, placed: 0, direction: 'horizontal' },
      { size: 2, count: 3, placed: 0, direction: 'horizontal' },
      { size: 1, count: 4, placed: 0, direction: 'horizontal' }
    ];
    this.currentShip = null;
    this.placementBoard = Array(10).fill().map(() => Array(10).fill(null));
    this.myBoard = Array(10).fill().map(() => Array(10).fill(null));
    this.enemyBoard = Array(10).fill().map(() => Array(10).fill(null));
    this.isMyTurn = false;
    this.isChatOpen = true;
    
    this.elements = {
      startScreen: document.getElementById('start-screen'),
      waitingScreen: document.getElementById('waiting-screen'),
      placementScreen: document.getElementById('placement-screen'),
      gameScreen: document.getElementById('game-screen'),
      roomIdDisplay: document.getElementById('room-id-display'),
      currentRoomId: document.getElementById('current-room-id'),
      playerIndicator: document.getElementById('player-indicator'),
      placementBoard: document.getElementById('placement-board'),
      myBoard: document.getElementById('my-board'),
      enemyBoard: document.getElementById('enemy-board'),
      gameStatus: document.getElementById('game-status'),
      donePlacementBtn: document.getElementById('done-placement-btn'),
      rotateBtn: document.getElementById('rotate-btn'),
      randomPlaceBtn: document.getElementById('random-place-btn'),
      restartBtn: document.getElementById('restart-btn'),
      leaveBtn: document.getElementById('leave-btn'),
      roomIdInput: document.getElementById('room-id-input'),
      chatMessages: document.getElementById('chat-messages'),
      chatInput: document.getElementById('chat-input'),
      sendChatBtn: document.getElementById('send-chat-btn'),
      toggleChatBtn: document.getElementById('toggle-chat-btn')
    };
  }

  setupEventListeners() {
    // Кнопки начального экрана
    document.getElementById('create-room-btn').addEventListener('click', () => this.createRoom());
    document.getElementById('join-room-btn').addEventListener('click', () => this.joinRoom());
    this.elements.roomIdInput.addEventListener('keypress', (e) => {
      if (e.key === 'Enter') this.joinRoom();
    });
    
    // Кнопки расстановки
    this.elements.donePlacementBtn.addEventListener('click', () => this.finishPlacement());
    this.elements.rotateBtn.addEventListener('click', () => this.rotateShip());
    this.elements.randomPlaceBtn.addEventListener('click', () => this.randomPlacement());
    
    // Кнопки во время игры
    this.elements.leaveBtn.addEventListener('click', () => this.leaveGame());
    this.elements.restartBtn.addEventListener('click', () => this.restartGame());
    
    // Чат
    this.elements.sendChatBtn.addEventListener('click', () => this.sendChatMessage());
    this.elements.chatInput.addEventListener('keypress', (e) => {
      if (e.key === 'Enter') this.sendChatMessage();
    });
    this.elements.toggleChatBtn.addEventListener('click', () => this.toggleChat());
    
    // Копирование ID комнаты
    document.getElementById('copy-room-id').addEventListener('click', () => this.copyRoomId());
  }
  
  checkForRoomId() {
    const urlParams = new URLSearchParams(window.location.search);
    const roomId = urlParams.get('roomId');
    
    if (roomId) {
      this.roomId = roomId;
      this.connectToRoom(roomId);
      this.showWaitingScreen();
      this.elements.roomIdDisplay.textContent = roomId;
      this.elements.currentRoomId.textContent = roomId;
    }
  }
  
  createRoom() {
    this.roomId = this.generateRoomId();
    this.updateUrl();
    this.connectToRoom(this.roomId);
    this.showWaitingScreen();
  }
  
  joinRoom() {
    const roomId = this.elements.roomIdInput.value.trim().toUpperCase();
    if (!roomId) return;
    
    this.roomId = roomId;
    this.updateUrl();
    this.connectToRoom(roomId);
    this.showWaitingScreen();
  }
  
  generateRoomId() {
    return Math.random().toString(36).substring(2, 8).toUpperCase();
  }
  
  updateUrl() {
    history.pushState({}, '', `?roomId=${this.roomId}`);
    this.elements.roomIdDisplay.textContent = this.roomId;
    this.elements.currentRoomId.textContent = this.roomId;
  }
  
  connectToRoom(roomId) {
    const wsProtocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    this.socket = new WebSocket(`${wsProtocol}//${window.location.host}?roomId=${roomId}`);
    
    this.socket.onopen = () => {
      console.log('Connected to WebSocket server');
    };
    
    this.socket.onmessage = (event) => {
      const message = JSON.parse(event.data);
      this.handleServerMessage(message);
    };
    
    this.socket.onclose = () => {
      console.log('Disconnected from server');
      this.showSystemMessage('Соединение с сервером потеряно. Перезагрузите страницу.');
      if (this.gameState !== 'start') {
        this.showStartScreen();
      }
    };
    
    this.socket.onerror = (error) => {
      console.error('WebSocket error:', error);
    };
  }
  
  handleServerMessage(message) {
    switch (message.type) {
      case 'joined':
        this.playerId = message.playerId;
        this.playerIndex = message.playerIndex;
        this.updatePlayerIndicator();
        break;
        
      case 'playerJoined':
        this.showSystemMessage(`Игрок ${message.playerIndex + 1} присоединился к игре`);
        if (message.playerIndex === 1) {
          this.showPlacementScreen();
        }
        break;
        
      case 'startPlacing':
        this.showPlacementScreen();
        break;
        
      case 'playerReady':
        this.showSystemMessage(`Игрок ${message.playerIndex + 1} готов`);
        if (message.playerIndex === this.playerIndex) {
          this.elements.donePlacementBtn.textContent = 'Ожидание противника...';
          this.elements.donePlacementBtn.classList.add('disabled');
          this.elements.donePlacementBtn.disabled = true;

          this.elements.randomPlaceBtn.classList.add('disabled');
          this.elements.randomPlaceBtn.disabled = true;
        }
        break;
        
      case 'startGame':
        this.startGame(message.currentPlayer, message.currentPlayerIndex);
        break;
        
      case 'fireResult':
        this.handleFireResult(message);
        break;
        
      case 'gameOver':
        this.handleGameOver(message);
        break;
        
      case 'chatMessage':
        this.displayChatMessage(message);
        break;
        
      case 'playerDisconnected':
        this.handlePlayerDisconnected(message);
        break;
        
      case 'error':
        this.showError(message.message);
        break;
    }
  }
  
  showStartScreen() {
    this.gameState = 'start';
    this.elements.startScreen.classList.remove('hidden');
    this.elements.waitingScreen.classList.add('hidden');
    this.elements.placementScreen.classList.add('hidden');
    this.elements.gameScreen.classList.add('hidden');
    document.querySelector('#room-info').classList.add('hidden');

    this.elements.restartBtn.classList.add('hidden');
  }
  
  showWaitingScreen() {
    this.gameState = 'waiting';
    this.elements.startScreen.classList.add('hidden');
    this.elements.waitingScreen.classList.remove('hidden');
    this.elements.placementScreen.classList.add('hidden');
    this.elements.gameScreen.classList.add('hidden');
    document.querySelector('#room-info').classList.remove('hidden');
  }
  
  showPlacementScreen() {
    this.gameState = 'placing';
    this.elements.startScreen.classList.add('hidden');
    this.elements.waitingScreen.classList.add('hidden');
    this.elements.placementScreen.classList.remove('hidden');
    this.elements.gameScreen.classList.add('hidden');
    
    this.placementBoard = Array(10).fill().map(() => Array(10).fill(null));
    
    // Полный сброс структуры кораблей к начальному состоянию
    this.shipsToPlace = [
      { size: 4, count: 1, placed: 0, direction: 'horizontal' },
      { size: 3, count: 2, placed: 0, direction: 'horizontal' },
      { size: 2, count: 3, placed: 0, direction: 'horizontal' },
      { size: 1, count: 4, placed: 0, direction: 'horizontal' }
    ];

    this.currentShip = this.shipsToPlace[0];

    this.elements.donePlacementBtn.textContent = 'Готово';
    this.elements.donePlacementBtn.classList.add('disabled');
    this.elements.donePlacementBtn.disabled = true;
    
    this.elements.randomPlaceBtn.classList.remove('disabled');
    this.elements.randomPlaceBtn.disabled = false;

    this.renderPlacementBoard();
    this.updatePlacementUI();
  }
  
  showGameScreen() {
    this.gameState = 'playing';
    this.elements.startScreen.classList.add('hidden');
    this.elements.waitingScreen.classList.add('hidden');
    this.elements.placementScreen.classList.add('hidden');
    this.elements.gameScreen.classList.remove('hidden');
    
    this.renderGameBoards();
  }
  
  updatePlayerIndicator() {
    this.elements.playerIndicator.textContent = `Вы: Игрок ${this.playerIndex + 1}`;
    this.elements.playerIndicator.className = `player-indicator player-${this.playerIndex}`;
  }
  
  renderPlacementBoard() {
    this.elements.placementBoard.innerHTML = '';
    
    for (let y = 0; y < 10; y++) {
      for (let x = 0; x < 10; x++) {
        const cell = document.createElement('div');
        cell.className = 'cell';
        cell.dataset.x = x;
        cell.dataset.y = y;
        
        if (this.placementBoard[y][x] === 'ship') {
          cell.classList.add('ship');
        }
        
        cell.addEventListener('click', () => this.placeShip(x, y));
        
        this.elements.placementBoard.appendChild(cell);
      }
    }
  }
  
  placeShip(x, y) {
    if (!this.currentShip || this.currentShip.placed >= this.currentShip.count) return;
    
    const size = this.currentShip.size;
    const direction = this.currentShip.direction;
    
    // Проверка выхода за границы
    if (direction === 'horizontal' && x + size > 10) return;
    if (direction === 'vertical' && y + size > 10) return;
    
    // Проверка пересечений
    for (let i = 0; i < size; i++) {
      const cx = direction === 'horizontal' ? x + i : x;
      const cy = direction === 'vertical' ? y + i : y;
      
      if (this.placementBoard[cy][cx] === 'ship') {
        this.showError('Корабли не могут пересекаться');
        return;
      }
      
      // Проверка вокруг корабля (правило одного пробела)
      for (let dy = -1; dy <= 1; dy++) {
        for (let dx = -1; dx <= 1; dx++) {
          const nx = cx + dx;
          const ny = cy + dy;
          
          if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
            if (this.placementBoard[ny][nx] === 'ship') {
              this.showError('Корабли должны находиться на расстоянии минимум одной клетки');
              return;
            }
          }
        }
      }
    }
    
    // Размещение корабля
    for (let i = 0; i < size; i++) {
      const cx = direction === 'horizontal' ? x + i : x;
      const cy = direction === 'vertical' ? y + i : y;
      this.placementBoard[cy][cx] = 'ship';
    }
    
    // Обновление данных о корабле
    this.currentShip.placed++;
    
    // Переход к следующему кораблю, если текущий полностью размещен
    if (this.currentShip.placed >= this.currentShip.count) {
      const currentIndex = this.shipsToPlace.findIndex(s => s === this.currentShip);
      if (currentIndex < this.shipsToPlace.length - 1) {
        this.currentShip = this.shipsToPlace[currentIndex + 1];
      } else {
        this.currentShip = null;
      }
    }
    
    this.renderPlacementBoard();
    this.updatePlacementUI();
    this.checkPlacementComplete();
  }
  
  rotateShip() {
    if (!this.currentShip) return;
    
    this.currentShip.direction = this.currentShip.direction === 'horizontal' ? 'vertical' : 'horizontal';
    this.updatePlacementUI();
  }
  
  randomPlacement() {
    // Очистка текущего поля
    this.placementBoard = Array(10).fill().map(() => Array(10).fill(null));
    this.shipsToPlace.forEach(ship => ship.placed = 0);
    
    // Сброс состояния кораблей
    this.shipsToPlace = [
      { size: 4, count: 1, placed: 0, direction: 'horizontal' },
      { size: 3, count: 2, placed: 0, direction: 'horizontal' },
      { size: 2, count: 3, placed: 0, direction: 'horizontal' },
      { size: 1, count: 4, placed: 0, direction: 'horizontal' }
    ];
    
    this.currentShip = this.shipsToPlace[0];
    
    // Автоматическая расстановка
    const shipsToPlace = [];
    shipsToPlace.push({ size: 4, count: 1 });
    shipsToPlace.push({ size: 3, count: 2 });
    shipsToPlace.push({ size: 2, count: 3 });
    shipsToPlace.push({ size: 1, count: 4 });
    
    for (const shipType of shipsToPlace) {
      for (let i = 0; i < shipType.count; i++) {
        let placed = false;
        let attempts = 0;
        
        while (!placed && attempts < 100) {
          attempts++;
          const direction = Math.random() > 0.5 ? 'horizontal' : 'vertical';
          const maxX = direction === 'horizontal' ? 10 - shipType.size : 9;
          const maxY = direction === 'vertical' ? 10 - shipType.size : 9;
          const x = Math.floor(Math.random() * (maxX + 1));
          const y = Math.floor(Math.random() * (maxY + 1));
          
          let canPlace = true;
          
          // Проверка возможности размещения
          for (let i = 0; i < shipType.size; i++) {
            const cx = direction === 'horizontal' ? x + i : x;
            const cy = direction === 'vertical' ? y + i : y;
            
            if (this.placementBoard[cy][cx] === 'ship') {
              canPlace = false;
              break;
            }
            
            // Проверка вокруг корабля
            for (let dy = -1; dy <= 1; dy++) {
              for (let dx = -1; dx <= 1; dx++) {
                const nx = cx + dx;
                const ny = cy + dy;
                
                if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
                  if (this.placementBoard[ny][nx] === 'ship') {
                    canPlace = false;
                    break;
                  }
                }
              }
              if (!canPlace) break;
            }
            if (!canPlace) break;
          }
          
          if (canPlace) {
            // Размещение корабля
            for (let i = 0; i < shipType.size; i++) {
              const cx = direction === 'horizontal' ? x + i : x;
              const cy = direction === 'vertical' ? y + i : y;
              this.placementBoard[cy][cx] = 'ship';
            }
            
            // Обновление счетчика для конкретного типа корабля
            const shipConfig = this.shipsToPlace.find(s => s.size === shipType.size);
            if (shipConfig) shipConfig.placed++;
            
            placed = true;
          }
        }
      }
    }
    
    // Обновление текущего корабля
    this.currentShip = this.shipsToPlace.find(ship => ship.placed < ship.count) || null;
    
    this.renderPlacementBoard();
    this.updatePlacementUI();
    this.checkPlacementComplete();
  }
  
  updatePlacementUI() {
    if (!this.currentShip) {
      this.elements.donePlacementBtn.classList.remove('disabled');
      this.elements.donePlacementBtn.disabled = false;
      this.elements.rotateBtn.disabled = true;
      return;
    }
    
    this.elements.rotateBtn.disabled = false;
    this.elements.rotateBtn.textContent = `↺ Повернуть (${this.currentShip.direction === 'horizontal' ? 'Горизонтально' : 'Вертикально'})`;
    
    const totalShips = this.shipsToPlace.reduce((sum, ship) => sum + ship.count, 0);
    const placedShips = this.shipsToPlace.reduce((sum, ship) => sum + ship.placed, 0);
    
    if (placedShips === totalShips) {
      this.elements.donePlacementBtn.classList.remove('disabled');
      this.elements.donePlacementBtn.disabled = false;
    }
  }
  
  checkPlacementComplete() {
    const totalShips = this.shipsToPlace.reduce((sum, ship) => sum + ship.count, 0);
    const placedShips = this.shipsToPlace.reduce((sum, ship) => sum + ship.placed, 0);
    
    if (placedShips === totalShips) {
      this.elements.donePlacementBtn.classList.remove('disabled');
      this.elements.donePlacementBtn.disabled = false;
    }
  }
  
  finishPlacement() {
    // Сбор данных о кораблях для отправки на сервер
    const ships = [];
    const tempBoard = Array(10).fill().map(() => Array(10).fill(null));
    
    // Копирование placementBoard в tempBoard
    for (let y = 0; y < 10; y++) {
      for (let x = 0; x < 10; x++) {
        tempBoard[y][x] = this.placementBoard[y][x];
      }
    }
    
    // Поиск кораблей на поле
    for (let y = 0; y < 10; y++) {
      for (let x = 0; x < 10; x++) {
        if (tempBoard[y][x] === 'ship') {
          // Определение направления корабля
          let direction = 'horizontal';
          let size = 1;
          
          // Проверка вправо (горизонтальное направление)
          for (let i = 1; x + i < 10 && tempBoard[y][x + i] === 'ship'; i++) {
            size++;
            tempBoard[y][x + i] = null; // Помечаем как обработанную
          }
          
          // Если не нашли корабль вправо, проверяем вниз (вертикальное направление)
          if (size === 1) {
            direction = 'vertical';
            for (let i = 1; y + i < 10 && tempBoard[y + i][x] === 'ship'; i++) {
              size++;
              tempBoard[y + i][x] = null; // Помечаем как обработанную
            }
          }
          
          // Добавление корабля в список
          ships.push({
            x: x,
            y: y,
            size: size,
            direction: direction
          });
          
          // Помечаем текущую клетку как обработанную
          tempBoard[y][x] = null;
        }
      }
    }
    
    // Отправка данных на сервер
    this.socket.send(JSON.stringify({
      type: 'placeShips',
      ships: ships
    }));
    
    // Обновление состояния
    this.myBoard = JSON.parse(JSON.stringify(this.placementBoard));
    this.showSystemMessage('Корабли расставлены. Ожидание противника...');
  }
  
  startGame(currentPlayer, currentPlayerIndex) {
    this.isMyTurn = (currentPlayer === this.playerId);
    this.updateGameStatus();
    this.showGameScreen();
    this.showSystemMessage(this.isMyTurn ? 'Ваш ход!' : 'Ход противника');
  }
  
  renderGameBoards() {
    this.renderBoard(this.elements.myBoard, this.myBoard, false);
    this.renderBoard(this.elements.enemyBoard, this.enemyBoard, true);
  }
  
  renderBoard(boardElement, boardData, isEnemy) {
    boardElement.innerHTML = '';
    
    for (let y = 0; y < 10; y++) {
      for (let x = 0; x < 10; x++) {
        const cell = document.createElement('div');
        cell.className = 'cell';
        cell.dataset.x = x;
        cell.dataset.y = y;
        
        if (boardData[y][x] === 'ship' && !isEnemy) {
          cell.classList.add('ship');
        } else if (boardData[y][x] === 'hit') {
          cell.classList.add('hit');
        } else if (boardData[y][x] === 'miss') {
          cell.classList.add('miss');
        }
        
        if (isEnemy && this.isMyTurn && boardData[y][x] === null) {
          cell.addEventListener('click', () => this.fireAt(x, y));
        }
        
        boardElement.appendChild(cell);
      }
    }
  }
  
  fireAt(x, y) {
    if (!this.isMyTurn || this.enemyBoard[y][x] !== null) return;
    
    this.socket.send(JSON.stringify({
      type: 'fire',
      x: x,
      y: y
    }));
    
    // Визуальная обратная связь
    const cell = this.elements.enemyBoard.querySelector(`[data-x="${x}"][data-y="${y}"]`);
    if (cell) {
      cell.style.background = 'rgba(52, 152, 219, 0.3)';
    }
  }
  
  handleFireResult(message) {
    const { x, y, hit, gameOver, winner, prevPlayer, prevPlayerIndex, currentPlayer, currentPlayerIndex } = message;
    
    // Обновление поля противника
    if (this.playerId === winner && gameOver) {
      // Если мы победили, открываем все корабли противника
      for (let y = 0; y < 10; y++) {
        for (let x = 0; x < 10; x++) {
          if (this.myBoard[y][x] === 'ship' && this.enemyBoard[y][x] === null) {
            this.enemyBoard[y][x] = 'ship';
          }
        }
      }
    }
    
    if (this.playerIndex === prevPlayerIndex) {
      // Это наш выстрел - обновляем поле противника
      this.enemyBoard[y][x] = hit ? 'hit' : 'miss';
    } else {
      // Это выстрел противника - обновляем наше поле
      this.myBoard[y][x] = hit ? 'hit' : 'miss';
    }
    
    if (gameOver) {
      this.gameState = 'finished';
      this.elements.restartBtn.classList.remove('hidden');
      this.renderGameBoards();
      return;
    }

    this.isMyTurn = (this.playerId === currentPlayer);
    this.updateGameStatus();
    this.renderGameBoards();
  }
  
  updateGameStatus() {
    if (this.gameState === 'finished') return;
    
    this.elements.gameStatus.textContent = this.isMyTurn ? 'Ваш ход' : 'Ход противника';
    this.elements.gameStatus.className = this.isMyTurn ? 'my-turn' : 'enemy-turn';
  }
  
  handleGameOver(message) {
    const { winner, winnerIndex, reason } = message;
    
    this.gameState = 'finished';
    this.elements.restartBtn.classList.remove('hidden');
    
    if (winner === this.playerId) {
      this.elements.gameStatus.textContent = 'Вы победили! 🎉';
      this.elements.gameStatus.className = 'game-won';
    } else {
      this.elements.gameStatus.textContent = 'Вы проиграли 😢';
      this.elements.gameStatus.className = 'game-lost';
    }
    
    if (reason) {
      this.showSystemMessage(reason);
    }
    
    // Показать все корабли противника
    for (let y = 0; y < 10; y++) {
      for (let x = 0; x < 10; x++) {
        if (this.myBoard[y][x] === 'ship' && this.enemyBoard[y][x] === null) {
          this.enemyBoard[y][x] = 'ship';
        }
      }
    }
    
    this.renderGameBoards();
  }
  
  handlePlayerDisconnected(message) {
    this.showSystemMessage(message.message);
    
    if (this.gameState === 'playing' || this.gameState === 'placing') {
      this.elements.gameStatus.textContent = 'Противник отключился';
      this.elements.gameStatus.className = 'game-lost';
      this.elements.restartBtn.classList.remove('hidden');
    }
    
    if (this.gameState === 'waiting') {
      this.showStartScreen();
    }
  }
  
  leaveGame() {
    if (this.socket) {
      this.socket.close();
    }
    this.showStartScreen();
    this.setDefault();
    history.pushState({}, '', '/');
  }
  
  restartGame() {
    // Перезагрузка страницы для создания новой игры
    this.setDefault();
    window.location.reload();
  }
  
  sendChatMessage() {
    const message = this.elements.chatInput.value.trim();
    if (!message || !this.socket) return;
    
    this.socket.send(JSON.stringify({
      type: 'chatMessage',
      text: message
    }));
    
    this.elements.chatInput.value = '';
  }
  
  displayChatMessage(message) {
    const chatMessage = document.createElement('div');
    chatMessage.className = 'chat-message';
    
    const time = new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    
    if (message.type === 'system') {
      chatMessage.classList.add('system');
      chatMessage.innerHTML = `<span class="time">[${time}]</span> ${message.text}`;
    } else {
      const isSelf = message.playerId === this.playerId;
      chatMessage.classList.add(isSelf ? 'self' : 'other');
      
      const playerName = isSelf ? 'Вы' : `Игрок ${message.playerIndex + 1}`;
      chatMessage.innerHTML = `
        <div class="message-header">
          <span class="player-name player-${message.playerIndex}">${playerName}</span>
          <span class="time">[${time}]</span>
        </div>
        <div class="message-content">${this.escapeHtml(message.message)}</div>
      `;
    }
    
    this.elements.chatMessages.appendChild(chatMessage);
    this.elements.chatMessages.scrollTop = this.elements.chatMessages.scrollHeight;
  }
  
  showSystemMessage(message) {
    this.displayChatMessage({
      type: 'system',
      text: message
    });
  }
  
  toggleChat() {
    // 1. Получаем контейнер сообщений (уже есть в this.elements)
    const messagesContainer = this.elements.chatMessages; 
    
    // 2. Получаем контейнер ввода (родитель элемента chatInput)
    const inputArea = this.elements.chatInput.parentElement;
    
    // 3. Получаем основной контейнер чата, чтобы управлять его размером
    const chatPanel = document.querySelector('.chat-panel');

    this.isChatOpen = !this.isChatOpen;
    
    if (this.isChatOpen) {
        // ПОКАЗАТЬ: Устанавливаем display и высоту, чтобы чат занимал свое место
        messagesContainer.style.display = 'flex';
        inputArea.style.display = 'flex';
        chatPanel.style.height = '600px'; // Восстанавливаем высоту, заданную в CSS
        
        this.elements.toggleChatBtn.textContent = 'Скрыть';
    } else {
        // СКРЫТЬ: Скрываем содержимое и уменьшаем высоту chat-panel
        messagesContainer.style.display = 'none';
        inputArea.style.display = 'none';
        
        // Устанавливаем минимальную высоту, чтобы был виден только заголовок
        chatPanel.style.height = '80px';

        this.elements.toggleChatBtn.textContent = 'Показать чат';
    }
  }
  
  copyRoomId() {
    navigator.clipboard.writeText(this.roomId).then(() => {
      const btn = document.getElementById('copy-room-id');
      const originalText = btn.textContent;
      btn.textContent = 'Скопировано!';
      setTimeout(() => {
        btn.textContent = originalText;
      }, 2000);
    });
  }
  
  showError(message) {
    this.showSystemMessage(`Ошибка: ${message}`);
    console.error('Game error:', message);
  }
  
  escapeHtml(unsafe) {
    return unsafe
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/"/g, "&quot;")
      .replace(/'/g, "&#039;");
  }
}

// Запуск игры при загрузке страницы
document.addEventListener('DOMContentLoaded', () => {
  new SeaBattleGame();
});