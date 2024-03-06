/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:12:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/06 10:32:40 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Server.hpp"

# define PORT 8080


	
// use getaddrinfo()
	// bzero(&S_Addr, sizeof(S_Addr));
	// S_Addr.sin_family = AF_INET;// * we use ipv4
	// S_Addr.sin_addr.s_addr = INADDR_ANY;//*the OS set to my macine's IP address//inet_addr("10.13.10.4"); u don't need to use htonl() , becouse I set 0.0.0.0
	// std::cout << "here: " << vSer->getPort() << "\n";
	// S_Addr.sin_port = htons(vSer->getPort()); //* host to network short (little endian / big endian problem)
int Server::socketCreate(VirtualServer* vSer)
{
	// struct sockaddr_in  S_Addr;
	int                 sockfd;
	
	struct addrinfo	hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	std::cout << "check : " << vSer->getHost() << " " << vSer->getPort() << "\n";
	getaddrinfo((vSer->getHost()).c_str(), (vSer->getPort()).c_str(), &hints, &res);
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
		throw std::runtime_error("socket() failed\n");
	std::cout << "server: " << sockfd << " created\n";
	int on = 1;
// SO_REUSEADDR is used to enable the reusing of local addresses in the bind() function.
// setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) //* so you can reuse port num
		throw std::runtime_error("setsockopt() failed\n");
		
	
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
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
		bzero(&event, sizeof(event));
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

	std::cout << "wait for a cleint\n";
	events[index].data.fd = accept(serverFd, NULL, &clientAddrLen);
	if (events[index].data.fd < 0)
		throw std::runtime_error("accept\n");
	std::cout << "Cleint fd: " << events[index].data.fd << " accepted\n";
// accept connection , add client to epoll, parse request
	clients[events[index].data.fd] = new Client(events[index].data.fd, Vservers[index], &files);
	event.data.fd = events[index].data.fd;
	event.events = EPOLLIN;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, events[index].data.fd, &event) == -1)
		throw std::runtime_error("epoll_ctl [Cleint]");
	std::cout << RED << "done\n" << RESET << std::endl;

}

bool	Server::NewClient(int index)
{
	for (size_t j = 0; j < Vservers.size(); j++) // add the new Cleint to epoll
	{
		if (events[index].data.fd == Vservers[j]->getFdSocket())
		{
			Server::addCleintToEpoll(j);
			return (1);
		}
	}
	return (0);
}

int Server::launchServer()
{

	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw std::runtime_error("epoll_create1 failed");
// add fd_servers to the epoll
	addServersToEpoll();

	while (true)
	{
		int readyFd;
		if ((readyFd = epoll_wait(epollFd, events, MAX_EVENTS, 0)) != 0)
		{
			std::cout << "***************** wiating for a new connection *******************\n";
			if (readyFd < 0)
				throw std::runtime_error("epoll_wait error");
			for (int i = 0; i < readyFd; i++)
			{
				if (NewClient(i))
				{
					std::cout << "add new Cleint\n";
				}
				else if (clients[events[i].data.fd]->DoneHeaderReading == false) // read the header request
				{
					// read the header
					clients[events[i].data.fd]->ReadParseReqHeader();
					std::cout << YELLOW << "READ HEADER\n" << RESET << std::endl;
				}
				else
				{
					if (clients[events[i].data.fd]->DoneServing == false) // serve the client
					{
						std::cout << YELLOW << "SERVING Client" << RESET << std::endl;
						clients[events[i].data.fd]->ServingClient();
					// serve (Get, Post, delete)
						
					}
					else 
					{
						//DropCleint(events[i].data.fd);
					// delete the fdSoketCleint from the poll , and delete the cleint from the map
					}
				}
				// serve cleint
			}
		}
	}
}

Server::Server(std::vector<VirtualServer*>& Vser) : Vservers(Vser)
{
	files = getDataFromFiles();
}


Server::~Server()
{
	
}