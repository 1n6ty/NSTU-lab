#pragma comment(lib, "ws2_32.lib") //динамическая библиотека ядра
#include <WinSock2.h> //заголовочный файл, содержащий актуальные реализации 
                      //функций для работы с сокетами
#include <WS2tcpip.h> //заголовочный файл, который содержит различные программные интерфейсы, 
                      //связанные с работой протокола TCP/IP 
                      //(переводы различных данных в формат, понимаемый протоколом и т.д.)*/
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

int main(){

    //Инициализация Winsock
    WSADATA wsaData;
    // Проверка, успешно ли инициализировалась WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed\n";
        return 1;
    }

    //Создание сокета 
    SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Проверка, успешно ли создан сокет
    if (clientSock == INVALID_SOCKET) {
        std::cout << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in servInfo;
    servInfo.sin_family = AF_INET;
    servInfo.sin_port = htons(2001);
    
    // Ввод IP-адреса сервера (Hamachi IP)
    std::string serverIp;
    std::cout << "Enter server Hamachi IP: ";
    std::cin >> serverIp;
    std::cin.ignore();  // Очистка буфера
    
    servInfo.sin_addr.s_addr = inet_addr(serverIp.c_str());
    // Подключение к серверу
   std::cout << "Connecting to " << serverIp << ":2001...\n";
    
    if (connect(clientSock, (sockaddr*)&servInfo, sizeof(servInfo)) == SOCKET_ERROR) {
        std::cout << "Connection failed: " << WSAGetLastError() << "\n";
        std::cout << "Make sure:\n";
        std::cout << "1. Server is running\n";
        std::cout << "2. Hamachi is active on both computers\n";
        std::cout << "3. Computers are in the same Hamachi network\n";
        std::cout << "4. Firewall allows connections on port 2001\n";
        closesocket(clientSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Successfully connected to server!\n\n";

    // Ввод и отправка данных
    std::string input;
    std::cout << "Enter text to process: ";
    std::getline(std::cin, input);

    // Отправка данных серверу
    int sendResult = send(clientSock, input.c_str(), input.length(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cout << "Send failed: " << WSAGetLastError() << "\n";
        closesocket(clientSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Data sent to server\n";

    // Получение ответа от сервера
    char buffer[1024];
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "\nServer response: " << buffer << "\n";
    } else if (bytesReceived == 0) {
        std::cout << "Server disconnected\n";
    } else {
        std::cout << "Recv failed: " << WSAGetLastError() << "\n";
    }

    // Завершение работы
    closesocket(clientSock);
    WSACleanup();
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}