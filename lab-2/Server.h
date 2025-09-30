#pragma comment (lib, "Ws2_32.lib")
#include "winsock2.h"


class Server{
public:
	Server(unsigned short port);
	~Server();

	void start();
	void run();
	void close();

private:
	// Порт, на котором будет расположен сервер.
	unsigned short port;
	
	// Максимальное количиство одновременных TCP соединений.
	unsigned short const MAX_CON_NUM = 10;

	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN serverSIN;
	SOCKET clientSocket;
	SOCKADDR_IN clientSIN;

	unsigned int const BUF_SIZE = 256;

	// Буффер для взаимодействия с клиентами.
	char* buf;
};
