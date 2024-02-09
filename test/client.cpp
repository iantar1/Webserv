# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
#include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>


# define PORT 8080
# define CON_QUE 1

// Create a TCP server socket

int main() {

    struct sockaddr_in S_Addr;


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    S_Addr.sin_family = AF_INET;
    S_Addr.sin_addr.s_addr = inet_addr("10.13.10.4");//INADDR_ANY;//the OS set to my macine's IP address
    S_Addr.sin_port = htons(PORT);

    int addrlen = sizeof(S_Addr);
    if (connect(sockfd, (struct sockaddr*)&S_Addr, addrlen) == -1)
    {
        std::cerr << "conect failed with error code: " << errno << std::endl;
        close(sockfd);
        return 1;
    }

    while (true)
    {
        char buf[1024] = {0};
        char mes[1024] = {0};
        read(sockfd, buf, 1024);
        std::cout << buf << std::endl;
        read(0, mes, 1024);
        write(sockfd, mes, 1024);
        std::cout << "server: " << buf <<std::endl;
    }
        
    close(sockfd);
    return 0;
}