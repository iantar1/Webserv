/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/02/08 09:59:38 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/headers.hpp"

# define PORT 8080
# define CON_QUE 3

int main(int ac, char *av[])
{
    
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
    read(newSock, buf, 1024);
    std::cout << buf;
    parseRequest();
    close(sockfd);
    return (EXIT_SUCCESS);
}