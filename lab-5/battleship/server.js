import express from 'express';
import http from 'http';
import { WebSocketServer } from 'ws';

import {connectionManager} from './src/ConnectionManager.js';

const app = express();
const server = http.createServer(app);
const wss = new WebSocketServer({ server });

// Статические файлы для фронтенда
app.use(express.static('public'));

wss.on('connection', connectionManager);

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});