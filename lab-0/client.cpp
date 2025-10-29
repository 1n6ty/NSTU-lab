#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr{};
    char buffer[1024];
    const char* message = "Hello from UDP client!";
    int port = 8080;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout << "Sending message to server...\n";

    sendto(sockfd, message, strlen(message), 0,
           (const struct sockaddr*)&servaddr, sizeof(servaddr));

    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr*)&servaddr, &len);
    buffer[n] = '\0';
    std::cout << "Received: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
