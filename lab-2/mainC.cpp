#include "Client.h"
#include <iostream>
#include <string>

unsigned const int IP_STR_MAX_SIZE = 16;
unsigned const int PORT = 2001;

int main() {

    char *ip;
    ip = (char*)malloc(IP_STR_MAX_SIZE);
    
    std::cout << "ip> ";
    std::cin.getline(ip, IP_STR_MAX_SIZE);

    Client* client = new Client(ip, PORT);

    client->sendRequest();
    client->close();
    
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    free(ip);
    delete client;  // Добавлено освобождение памяти
    
    return 0;
}