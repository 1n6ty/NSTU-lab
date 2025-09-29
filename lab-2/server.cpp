#pragma comment(lib, "ws2_32.lib") //динамическая библиотека ядра
#include <WinSock2.h> //заголовочный файл, содержащий актуальные реализации 
                      //функций для работы с сокетами
#include <WS2tcpip.h> //заголовочный файл, который содержит различные программные интерфейсы, 
                      //связанные с работой протокола TCP/IP 
                      //(переводы различных данных в формат, понимаемый протоколом и т.д.)*/
#include <iostream>
#include <stdio.h>
#include <vector>

int main(){
  // Инициализация библиотеки WinSock (запрос версии 2.2)
  WSADATA wsData;
  int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
  // Проверка, успешно ли инициализировалась WinSock
  if ( erStat!= 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError() << "\n"; // Вывод кода ошибки
		return 1; // Завершение программы
	} else std::cout << "WinSock initialized successfully!\n";

  // Создание сокета
  // AF_INET -> используем IPv4
  // SOCK_STREAM -> создаём потоковый сокет (TCP)
  // 0 -> протокол по умолчанию (для TCP и UDP)
  SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);
  // Проверка, успешно ли создан сокет
  if (ServSock == INVALID_SOCKET){
    std::cout << "Ettor initialization socket # " << WSAGetLastError() << "\n";
    closesocket(ServSock); // Закрываем сокет (если он успел создаться)
    WSACleanup(); // Освобождаем ресурсы WinSock
    return 1;
  } else std::cout << "Socket initialized successfully!\n";

  // Настройка адреса сервера
  sockaddr_in servInfo;
  servInfo.sin_family = AF_INET;
  servInfo.sin_addr.s_addr = INADDR_ANY;  // Принимать подключения со всех интерфейсов
  servInfo.sin_port = htons(2001);  // Порт = номер бригады + 2000

  //Привязка сокета и проверка, успешно ли он привязался
  erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
  if (erStat != 0) {
    std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << "\n";
    closesocket(ServSock);
    WSACleanup();
  } else std::cout << "The socket was successfully bound!\n";

  // Переводим сокет в режим прослушивания
  erStat = listen(ServSock, 5);
  // Проверка, удалось ли начать прослушивание
  if( erStat != 0){
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << "\n";
		closesocket(ServSock); 
		WSACleanup();
		return 1;
  } else std::cout << "Server listening started successfully!\n";


  // Получение и вывод IP-адресов сервера
  char hostname[256];
  gethostname(hostname, sizeof(hostname));
  std::cout << "Server hostname: " << hostname << "\n";
    
  // Вывод всех IP-адресов
  hostent* host = gethostbyname(hostname);
  if (host != NULL) {
      std::cout << "Available IP addresses:" << "\n";
      for (int i = 0; host->h_addr_list[i] != 0; ++i) {
          sockaddr_in addr;
          memcpy(&addr.sin_addr, host->h_addr_list[i], host->h_length);
          std::cout << " - " << inet_ntoa(addr.sin_addr) << "\n";
      }
  } else {
      std::cout << "Cannot get host information" << "\n";
  }
    
  std::cout << "Server started on port 2001\n";
  std::cout << "Waiting for connections...\n";

  while (true) {
    // Принятие подключения
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    SOCKET clientSock = accept(ServSock, (sockaddr*)&clientAddr, &clientSize);
        
    if (clientSock == INVALID_SOCKET) {
      std::cout << "Accept failed: " << WSAGetLastError() << "\n";
      continue;
    }

    std::cout << "\n=== New connection accepted ===\n";
    std::cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << "\n";
    std::cout << "Client port: " << ntohs(clientAddr.sin_port) << "\n";

    // Получение данных от клиента
    char buffer[1024];
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        
    if (bytesReceived > 0) {
      buffer[bytesReceived] = '\0';
      std::string text(buffer);
            
      std::cout << "Received from client: " << text << "\n";

      // Вставка пробелов перед прописными буквами, если перед ними строчная
      std::string result;
      for (size_t i = 0; i < text.length(); ++i) {
        if (i > 0 && islower(text[i - 1]) && isupper(text[i])) {
          result += ' ';
        }
        result += text[i];
      }

      std::cout << "Processed result: " << result << "\n";

      // Отправка результата клиенту
      send(clientSock, result.c_str(), result.length(), 0);
      std::cout << "Response sent to client\n";
      } else if (bytesReceived == 0) {
        std::cout << "Client disconnected\n";
      } else {
        std::cout << "Recv failed: " << WSAGetLastError() << "\n";
      }

      closesocket(clientSock);
      std::cout << "Connection closed\n";
      std::cout << "Waiting for new connections...\n";
    }

    closesocket(ServSock);
    WSACleanup();
    return 0;
}