/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:12:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/09 17:29:59 by nabboune         ###   ########.fr       */
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
	int                 sockfd;
	
	struct addrinfo	hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;// IP4
	hints.ai_socktype = SOCK_STREAM; // stream TCP
	hints.ai_flags = AI_PASSIVE; // 
	hints.ai_protocol = IPPROTO_TCP; 

	std::cout << "Host : " << vSer->getHost() << " Port: " << vSer->getPort() << "\n";
	getaddrinfo((vSer->getHost()).c_str(), (vSer->getPort()).c_str(), &hints, &res);
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
		throw std::runtime_error("socket() failed\n");
	std::cout << "server: " << sockfd << " created\n";
	int on = 1;
// SO_REUSEADDR is used to enable the reusing of local addresses in the bind() function.
// setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
		
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on))) //* so you can reuse port num
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

// accept connection and add them to epoll

void    Server::addCleintToEpoll(int index)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	// std::cout << "waiting for a cleint" << "on : " << serverFd << "\n";
	int	fd = accept(Vservers[index]->getFdSocket(), NULL, &clientAddrLen);
	if (fd < 0)
		throw std::runtime_error("accept\n");
	// std::cout << "Cleint fd: " << fd << " accepted\n";

	clients[fd] = new Client(fd, Vservers[index], files);
	// std::cout << "fd: " << fd << "Client accepeted\n";
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLOUT;
	
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error("epoll_ctl [Cleint]");
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

void Server::DropCleint(int ClientFd)
{


	if (epoll_ctl(epollFd, EPOLL_CTL_DEL, ClientFd, NULL) == -1)
	{
		std::cerr << "Failed to remove client FD from epoll instance." << std::endl;
        close(epollFd);
		exit(1);
	}
	delete clients.find(ClientFd)->second;
	clients.erase(ClientFd);
	close(ClientFd);
	std::cout << RED << "Drop Client\n" << RESET;
}

// Desing Timeget.getResponse()


int Server::launchServer()
{
	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw std::runtime_error("epoll_create1 failed");
	addServersToEpoll();

	while (true)
	{
		int readyFd;
		bzero(events, sizeof(events));
		if ((readyFd = epoll_wait(epollFd, events, MAX_EVENTS, 0)) != 0)
		{
			// std::cout << "readyFd: " << readyFd << "\n";
			// std::cout << "***************** wiating for a new connection *******************\n";
			if (readyFd < 0)
				throw std::runtime_error("epoll_wait error");
			for (int i = 0; i < readyFd; i++)
			{
				// std::cout << "readyFD: " << readyFd << "\n";
				if (NewClient(i))
				{
					std::cout << YELLOW << "new Cleint added fd: "<< events[i].data.fd << "\n" << RESET;
				}
				else
				{
					if ((events[i].events & EPOLLIN) && clients[events[i].data.fd]->getDoneServing() == false) // serve the client
					{
						clients[events[i].data.fd]->ReadParseReqHeader();
					}
					else if ((events[i].events & EPOLLOUT) && clients[events[i].data.fd]->getDoneServing() == false)
					{
						clients[events[i].data.fd]->ServingClient();
					}
					else
					{
						DropCleint(events[i].data.fd);
					}
				}
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