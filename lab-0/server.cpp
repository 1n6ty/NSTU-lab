#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr{}, cliaddr{};
    char buffer[1024];
    int port = 8080;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    std::cout << "UDP server listening on port " << port << "...\n";

    socklen_t len = sizeof(cliaddr);

    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr*)&cliaddr, &len);
    if (n < 0) {
        perror("recvfrom");
        close(sockfd);
        return 1;
    }

    buffer[n] = '\0';
    std::cout << "Received: " << buffer << std::endl;

    const char* response = "Hello from UDP server!";
    sendto(sockfd, response, strlen(response), 0,
           (struct sockaddr*)&cliaddr, len);
    std::cout << "Sent response.\n";

    close(sockfd);
    return 0;
}
