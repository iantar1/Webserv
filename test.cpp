#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h> // Include this for non-blocking I/O
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h> // Include this for non-blocking I/O
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h> // Include this for error handling

const int PORT = 8080;
const int MAX_PENDING_CONNECTIONS = 5;
const int BUFFER_SIZE = 1024;

void handleConnection(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // Set socket to non-blocking mode
    int flags = fcntl(clientSocket, F_GETFL, 0);
    fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);

    // Read HTTP headers
    std::string headers;
    while ((bytesRead = read(clientSocket, buffer, BUFFER_SIZE)) > 0) {
        headers.append(buffer, bytesRead);
        if (headers.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    // Handle read errors
    if (bytesRead == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // No data available yet, but continue waiting
            std::cerr << "No data available yet" << std::endl;
        } else {
            // Error occurred
            std::cerr << "Error reading from socket: " << strerror(errno) << std::endl;
            close(clientSocket);
            return;
        }
    }

    // Extract content length from headers
    std::string contentLengthStr = "Content-Length: ";
    size_t contentLengthPos = headers.find(contentLengthStr);
    if (contentLengthPos == std::string::npos) {
        std::cerr << "Content-Length header not found" << std::endl;
        close(clientSocket);
        return;
    }

    size_t bodyStart = contentLengthPos + contentLengthStr.length();
    size_t contentLengthEnd = headers.find("\r\n", bodyStart);
    std::string contentLengthValue = headers.substr(bodyStart, contentLengthEnd - bodyStart);
    int contentLength = std::stoi(contentLengthValue);

    // Read request body and store in a file
    std::ofstream outputFile("received_image.jpg", std::ios::binary);
    int totalBytesRead = 0;
    while (totalBytesRead < contentLength) {
        bytesRead = read(clientSocket, buffer, std::min(BUFFER_SIZE, contentLength - totalBytesRead));
        if (bytesRead == 0) {
            // No more data available
            break;
        } else if (bytesRead < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // No data available yet, but continue waiting
            continue;
        } else if (bytesRead < 0) {
            // Error occurred
            std::cerr << "Error reading request body: " << strerror(errno) << std::endl;
            close(clientSocket);
            return;
        }
        outputFile.write(buffer, bytesRead);
        totalBytesRead += bytesRead;
    }

    std::cout << "Received image saved to received_image.jpg" << std::endl;

    outputFile.close();
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Bind to port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding to port " << PORT << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_PENDING_CONNECTIONS) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept incoming connections and handle them
    while (true) {
        clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        // Handle the connection in a separate thread or process
        handleConnection(clientSocket);
    }

    close(serverSocket);
    return 0;
}