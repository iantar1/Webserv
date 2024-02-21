/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/02/21 18:18:31 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/headers.hpp"

# define PORT 8080
# define CONNECTIN_QUEUE 100

// check errors



void    fillStruct(int port, struct sockaddr_in *S_Addr)
{
    S_Addr->sin_family = AF_INET;
    S_Addr->sin_addr.s_addr = INADDR_ANY;//*the OS set to my macine's IP address//inet_addr("10.13.10.4");
    S_Addr->sin_port = htons(port);
}

int launchServer(int port, struct sockaddr_in* S_Addr)
{
    //struct sockaddr_in S_Addr;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "socket: ";
        return (-1);
        //throw std::runtime_error("Socket creation failed");
    }

    fillStruct(port, S_Addr);

    //Allow socket descriptor to be reuseable  
    int on = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
    {
        std::cerr << "setsockopt() failed" << std::endl;
        exit(1);
    }
    
    int checkBind = bind(sockfd, (struct sockaddr*)(S_Addr), sizeof(*S_Addr));
    if (checkBind == -1)
    {
        std::cout << "bind: ";
        return (-1);
        close(sockfd);// you should
        throw std::runtime_error("Binding socket failed");
    }

    if (listen(sockfd, CONNECTIN_QUEUE) == -1)
    {
        std::cout << "lisen: ";
        return (-1);
        close(sockfd);
        throw std::runtime_error("lisen sys_call failed");
    }
    return (sockfd);
}

int main()
{
    struct sockaddr_in  S_Addr;
    int                 serverSoket;
    fd_set              readSocket;
    fd_set              writeSocket;
    fd_set              writeSocket_tmp;
    fd_set              readSocket_tmp;
    std::map<int, parseRequest*>    clients;
    int     readyFd;

    serverSoket = launchServer(PORT, &S_Addr);
    if (serverSoket < 0)    
        return ((std::cout << "lanchserver faild"), 1);
    int addrlen = sizeof(S_Addr);
    FD_ZERO(&writeSocket);
    //FD_SET(serverSoket, &writeSocket);

    FD_ZERO(&readSocket);
    FD_SET(serverSoket, &readSocket);
    // struct timeval  time;
    
    // time.tv_sec = 5;
    // time.tv_usec = 1;
    //int socketClinet;
    while (true)
    {
        readSocket_tmp = readSocket;
        writeSocket_tmp = writeSocket;
        std::cout << "waiting a connection\n";
        readyFd = 0;
        if ((readyFd = select(FD_SETSIZE, &readSocket_tmp, &writeSocket_tmp, NULL, NULL)) == -1)
        {
            std::cerr << "select failed" << std::endl;
            return (-1);
        }
        std::cout << "readyFd: " << readyFd << std::endl;
        for (int i = 0; i < FD_SETSIZE && readyFd > 0 ; i++)
        {
            if (FD_ISSET(i, &readSocket_tmp) || FD_ISSET(i, &writeSocket_tmp))
            {
                std::cout << "i = " << i << std::endl;
                if (i == serverSoket)
                {
                    int socketClinet = accept(serverSoket, (struct sockaddr*)(&S_Addr), (socklen_t*)&addrlen);
                    FD_SET(socketClinet, &readSocket);
                    //FD_SET(socketClinet, &writeSocket);
                    clients[socketClinet] = new parseRequest(socketClinet);
                    // clients[socketClinet]->readData();
                     std::cout << "before: " << i << " " << clients[i] << std::endl;
                     
                    readyFd--;
                    
                }
                else
                {
                    try
                    {
                        if (!clients[i]->reading_done)
                        {
                            clients[i]->readData();
                            FD_SET(i, &writeSocket);
                            clients[i]->reading_done = true;
                            FD_CLR(i, &readSocket);
                            
                            continue ;
                        }
                        std::cout << "after: " << i << " " << clients[i] << std::endl;
                       //clients[i]->readData();
                       //FD_SET(i, &writeSocket);
                       if (clients[i]->isDone == false)
                       {
                            clients[i]->getMethode();
                       }  
                       else
                       {
                            //FD_CLR(i, &writeSocket);
                            FD_CLR(i, &readSocket);
                            //delete[] clients[i];
                            //clients.erase(i);
                       }
                        //FD_SET(i, &writeSocket);
                       //clients[i]->printData();
                       readyFd--;
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                        
                    }
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}
/*
*/
# define MAX_EVENTS 100

int main(void)
{
   struct sockaddr_in               S_Addr;
    int                             serverSoket;
    std::map<int, parseRequest*>    clients;
    int                             epollFd;


    if ((serverSoket = launchServer(PORT, &S_Addr)) < 0)
    {
        return ((std::cout << "lanchserver faild"), 1);
    }
    
    if ((epollFd = epoll_create1(0)) == -1)
        return ((std::cout << "epoll_create1 faild"), 1);
    
    // add serverSoket fd to epoll
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSoket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSoket, &event) == -1)
        return ((std::cout << "epoll_ctl faild"), 1);
    int addrlen = sizeof(S_Addr);
    int readyFds;
    while (true)
    {
        if ((readyFds = epoll_wait(epollFd, &event, CONNECTIN_QUEUE, 0)) == -1)
            return((std::cout << "epoll_wait faild"), 1);
        if (readyFds > 0)
        {
            if (event.data.fd == serverSoket)
            {
                int newClient = accept(serverSoket, (struct sockaddr*)(&S_Addr), (socklen_t*)&addrlen);
                epoll_ctl(epollFd, EPOLL_CTL_ADD, newClient, &event);
                clients[newClient] = new parseRequest(newClient);
            }
            else
            {
                
            }
        }
        if ()
        {
            epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSoket, &event)
        }
        else
        {
        }
        if ()
        {
            // accept connection
        }
        else
        {
            // handel resposnse
            if ()
            {
                // read the request
            }
            else
            {
                // response
            }
        }
    }
    
    return (0);
}