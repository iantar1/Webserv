# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
#include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>
# include <fstream>
# include <sstream>

# define PORT 8080
# define CON_QUE 3

// Create a TCP server socket
/**
* TODO: kkjdck
* ! jflkdfhlkdf

*/
//* TODO: 

void sendHTML(int clientSocket) {
    // Open the HTML file
    std::ifstream file("index.html");
    if (!file.is_open()) {
        std::cerr << "Failed to open HTML file" << std::endl;
        return;
    }

    // Read the contents of the file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string htmlContent = buffer.str();

    // Prepare the HTTP response with the HTML content
    std::string httpResponse = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n"
                               "\r\n" + htmlContent;

    // Send the HTTP response to the client
    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
}


int main() {

    struct sockaddr_in S_Addr;


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    S_Addr.sin_family = AF_INET;
    S_Addr.sin_addr.s_addr = INADDR_ANY;//*the OS set to my macine's IP address//inet_addr("10.13.10.4");
    S_Addr.sin_port = htons(PORT);
    int man3rf = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &man3rf, sizeof(int));
    int checkBind = bind(sockfd, (struct sockaddr*)(&S_Addr), sizeof(S_Addr));
    if (checkBind == -1)
    {
        //std::cerr << "binding socket failed" << std::endl;
        std::cerr << "Binding socket failed with error code: " << errno << std::endl;
        close(sockfd);
        return 1;
    }
    if (listen(sockfd, CON_QUE) == -1)
    {
        std::cerr << "lisen sys_call failed with error code: " << errno << std::endl;
        close(sockfd);
        return 1;
    }
    int addrlen = sizeof(S_Addr);
    std::cout << "sever sockect fd: " << sockfd << std::endl;
    std::cout << "+++++++ Waiting for new connection ++++++++\n";
    int newSock;
    newSock = accept(sockfd, (struct sockaddr*)(&S_Addr), (socklen_t*)&addrlen);
    if (newSock < 0)
    {
        std::cerr << "accept sys_call failed with error code: " << errno << std::endl;
        close(sockfd);
        return 1;
    }
    char buf[1024] = {0};
    int sizeRead;
    sizeRead = read(newSock, buf, 1024);
    std::cout << buf;
    close(sockfd);
    return 0;
}
