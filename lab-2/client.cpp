#include "Client.h"
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>  // для strlen

Client::Client(const char* ip, unsigned short port) {
    this->ip = ip;
    this->port = port;

    int res = WSAStartup(0x0101, &wsaData);
    if (res != 0) {  // Исправлено: WSAStartup возвращает 0 при успехе
        std::cout << "WSAStartup() failed: " << res << "\n";
        return;
    }

    // Получаем дескриптор сокета клиента.
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {  // Исправлено: SOCKET_ERROR -> INVALID_SOCKET
        std::cout << "Failed to create client socket: " << WSAGetLastError() << "\n";
        return;
    }

    // Заполняем информацию о сокете сервера.
    serverSIN.sin_family = AF_INET;
    serverSIN.sin_port = htons(port);
    serverSIN.sin_addr.S_un.S_addr = inet_addr(ip); 

    buf = (char*)malloc(BUF_SIZE);
    if (buf == nullptr) {
        std::cout << "Memory allocation failed\n";
    }
}

Client::~Client() {
    if (buf != nullptr) {
        free(buf);
    }
}

// Отправляем запрос серверу.
void Client::sendRequest(){
    // Подключаемся к серверу.
    int res = connect(clientSocket, (LPSOCKADDR)&serverSIN, sizeof(serverSIN));
    if (res == SOCKET_ERROR) {
        std::cout << "Connection failed: " << WSAGetLastError() << "\n";
        std::cout << "Make sure:\n";
        std::cout << "1. Server is running\n";
        std::cout << "2. Correct IP and port: " << ip << ":" << port << "\n";
        std::cout << "3. Firewall allows connections\n";
        return;
    }
    std::cout << "Connected to server " << ip << ":" << port << " successfully!\n\n";

    // Ввод строки для обработки
    std::cout << "Enter text to process: ";
    std::cin.getline(buf, BUF_SIZE);

    // Отправляем строку серверу
    res = send(clientSocket, buf, strlen(buf), 0);
    if (res == SOCKET_ERROR) {
        std::cout << "Failed to send data: " << WSAGetLastError() << "\n";
        return;
    }
    std::cout << "Data sent to server.\n";

    // Очищаем буфер для приема ответа
    memset(buf, 0, BUF_SIZE);

    // Получаем обработанную строку от сервера
    res = recv(clientSocket, buf, BUF_SIZE - 1, 0);  // -1 для места под '\0'
    if (res == SOCKET_ERROR) {
        std::cout << "Server is not responding: " << WSAGetLastError() << "\n";
    }
    else if (res == 0) {
        std::cout << "Server closed connection\n";
    }
    else {
        buf[res] = '\0';  // Добавляем завершающий ноль
        std::cout << "Result: " << buf << "\n";
    }
}

void Client::close() {
    if (clientSocket != INVALID_SOCKET) {
        closesocket(clientSocket);
    }
    WSACleanup();
    std::cout << "Client closed.\n";
}