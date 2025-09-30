#include "Server.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <cctype>
#include <cstring>
#include <iostream>

Server::Server(unsigned short port) {
    this->port = port;

    int res = WSAStartup(0x0101, &wsaData);
    if (res != 0) {  // Исправлено: WSAStartup возвращает 0 при успехе
        std::cout << "WSAStartup() failed: " << res << "\n";
        Server::close();
        return;  // Добавлено: предотвращение дальнейшего выполнения при ошибке
    }
    
    // Присваиваем сокету дескриптор.
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {  // Исправлено: SOCKET_ERROR -> INVALID_SOCKET
        std::cout << "Failed to create server socket: " <<  WSAGetLastError()  << "\n";
        Server::close();
        return;
    }
    
    // Заполняем структуру с информацией о сокете сервера.
    serverSIN.sin_family = AF_INET;            // AF_INET определяет взаимодействие через интернет.
    serverSIN.sin_port = htons(port);          // Записываем порт.
    serverSIN.sin_addr.s_addr = INADDR_ANY;    // Привязываем сокет ко всем локальным IP.

    buf = (char*)malloc(BUF_SIZE);
    if (buf == nullptr) {  // Добавлена проверка выделения памяти
        std::cout << "Memory allocation failed\n";
    }
}

Server::~Server() {
    if (buf != nullptr) {  // Добавлена проверка перед освобождением
        free(buf);
    }
}

// Привязываем сервер к IP-адресу.
void Server::start() {
    int res = bind(serverSocket, (LPSOCKADDR)&serverSIN, sizeof(serverSIN));
    if (res == SOCKET_ERROR) {
        std::cout << "Failed to bind server: %d\n" << WSAGetLastError()  << "\n";
        Server::close();
        return;
    }
    std::cout << "Server binded to port " << port << "\n"; // Добавлен номер порта для информации
}

// Запускаем работу сервера.
void Server::run() {
    int res;
    sockaddr_in clientSIN;  // Добавлено: объявление структуры для клиента
    int clientSINSize = sizeof(clientSIN);
    
    while (true) {
        // Инициализируем слушающий сокет.
        res = listen(serverSocket, MAX_CON_NUM);
        if (res == SOCKET_ERROR) {
            std::cout << "Failed to start listen: " << WSAGetLastError() << "\n"; // Исправлено: GetLastError() -> WSAGetLastError()
            continue;
        }
        std::cout << "Listening for connections on port " << port << "\n";

        // Ожидаем запрос к серверу и записываем дескриптор сокета клиента.
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientSIN, &clientSINSize);  // Исправлено: добавлено объявление clientSocket
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "Failed to accept client: " << WSAGetLastError() << "\n"; // Исправлено: GetLastError() -> WSAGetLastError()
            continue;
        }
        std::cout << "Accept connection from " << inet_ntoa(clientSIN.sin_addr) << ", port " << ntohs(clientSIN.sin_port) << "\n";
        
        // Получаем данные от клиента.
        res = recv(clientSocket, buf, BUF_SIZE - 1, 0);  // Исправлено: BUF_SIZE -> BUF_SIZE-1 для места под '\0'

        if (res == SOCKET_ERROR) {
            // ошибка получения данных
            std::cout << "recv failed: " << WSAGetLastError() << "\n"; // Исправлено: GetLastError() -> WSAGetLastError()
            closesocket(clientSocket);
            continue;
        }
        else if (res == 0) {
            // соединение закрыто клиентом
            std::cout << "connection closed...\n";
            closesocket(clientSocket);
            continue;
        }
        else if (res > 0) {
            // Мы знаем фактический размер полученных данных,
            // поэтому ставим метку конца строки в буфере запроса.
            buf[res] = '\0';
            std::cout <<  "Received string: " << buf << "\n";
        }

        // Обработка текста по варианту задания
        std::string text(buf);  // Исправлено: создание строки из буфера
        std::string result;
        
        // Вставка пробелов перед прописными буквами, если перед ними строчная
        for (size_t i = 0; i < text.length(); ++i) {
            if (i > 0 && islower((unsigned char)text[i - 1]) && isupper((unsigned char)text[i])) {  // Исправлено: добавлено приведение типов
                result += ' ';
            }
            result += text[i];
        }
        
        // Отправка обработанного результата обратно клиенту
        res = send(clientSocket, result.c_str(), result.length(), 0);  // Исправлено: отправляем result, а не buf, убрано MSG_DONTROUTE

        if (res == SOCKET_ERROR) {
            std::cout << "send failed: " << WSAGetLastError() << "\n";
        } else {
            std::cout << "Sent response: " << result.c_str() << "\n";
        }

        closesocket(clientSocket);
        std::cout << "Client disconnected.\n";
    }
}

void Server::close() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    if (clientSocket != INVALID_SOCKET) {  // Исправлено: добавлена проверка
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
    WSACleanup();
    std::cout << "Server closed.\n";
}