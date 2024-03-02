/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:12:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 15:21:34 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Server.hpp"

// ?
// !
# define PORT 8080

// use getaddrinfo()
int Server::socketCreate(VirtualServer* vSer)
{
	// struct sockaddr_in  S_Addr;
	int                 sockfd;
	
	bzero(&S_Addr, sizeof(S_Addr));
	S_Addr.sin_family = AF_INET;// * we use ipv4
	S_Addr.sin_addr.s_addr = INADDR_ANY;//*the OS set to my macine's IP address//inet_addr("10.13.10.4"); u don't need to use htonl() , becouse I set 0.0.0.0
	S_Addr.sin_port = htons(PORT); //* host to network short (little endian / big endian problem)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("socket() failed\n");

	int on = 1;
// SO_REUSEADDR is used to enable the reusing of local addresses in the bind() function.
// setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) //* so you can reuse port num
		throw std::runtime_error("setsockopt() failed\n");
		
	
	if (bind(sockfd, (struct sockaddr*)(&S_Addr), sizeof(S_Addr)) < 0)
	{
		throw std::runtime_error("bind failed");
	}
	if (listen(sockfd, 100) == -1)
	{
		throw std::runtime_error("lisen sys_call failed");
	}
	vSer->setFdSocket(sockfd);
	return (sockfd);
}

void    Server::addServersToEpoll()
{
	for (size_t i = 0; i < Vservers.size(); i++)
	{
//		bzero(&event, sizeof(event));
		serverFd = socketCreate(Vservers[i]);
		event.data.fd = serverFd;
		event.events = EPOLLIN;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event) == -1)// * add an fd to the intrest list
		{
			throw std::runtime_error("epoll_ctl [Server]");
		}
	}
}

void    Server::addCleintToEpoll(int index)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	events[index].data.fd = accept(serverFd, (struct sockaddr*)(&clientAddr), &clientAddrLen);
	if (events[index].data.fd < 0)
		throw std::runtime_error("accept\n");
	clients[events[index].data.fd] = new Client(Vservers[index], events[index].data.fd);
	event.data.fd = events[index].data.fd;
	event.events = EPOLLIN;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, events[index].data.fd, &event) == -1)
		throw std::runtime_error("epoll_ctl [Cleint]");
	std::cout << RED << "done\n" << RESET << std::endl;
}


int Server::launchServer()
{
	epollFd = epoll_create1(0);
	
// add fd_servers to the epoll
	Server::addServersToEpoll();

	while (true)
	{
		int readyFd;
		if ((readyFd = epoll_wait(epollFd, events, MAX_EVENTS, 0)) > 0)
		{
			for (int i = 0; i < readyFd; i++)
			{
				for (size_t j = 0; j < Vservers.size(); j++) // add the new Cleint to epoll
				{
					if (events[i].data.fd == Vservers[j]->getFdSocket())
					{
						Server::addCleintToEpoll(j);
						break ;
					}
				}
			/*
			*/
				if (clients[events[i].data.fd]->DoneHeaderReading == false) // read the header request
				{
					// read the header
					clients[events[i].data.fd]->ReadParseReqHeader();
					std::cout << BLUE << "Ready TO READ MY HEADER\n" << RESET << std::endl;
				}
				else
				{
					if (clients[events[i].data.fd]->DoneHeaderReading == false) // serve the client
					{
					// serve (Get, Post, delete)
						
					}
					else 
					{
					// delete the fdSoketCleint from the poll , and delete the cleint from the map
					}
				}
				// serve cleint
			}
		}
	}
}

Server::Server(const std::vector<VirtualServer*>& Vser) : Vservers(Vser)
{
	
}


Server::~Server()
{
	
}