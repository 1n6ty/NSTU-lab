#pragma comment (lib, "Ws2_32.lib")
#include "winsock2.h"
#include <string>


class Client {
public:

	Client(const char* hostName, unsigned short port);
	~Client();

	void sendRequest();
	void close();

private:
	unsigned int const BUF_SIZE = 256;

	// IP-адрес сервера.
	std::string ip;
	// Порт сервера.
	unsigned short port;
	WSADATA wsaData;

	// Сокет клиента.
	SOCKET clientSocket;
	// Структура с информацией о сокете клиента.
	SOCKADDR_IN clientSIN;
	// Структура с информацией о сокете сервера.
	SOCKADDR_IN serverSIN;

	// Буфер для работы с сообщениями на стороне клиента.
	char* buf;
};
