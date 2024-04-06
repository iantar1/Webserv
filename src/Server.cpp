/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:12:09 by iantar            #+#    #+#             */
/*   Updated: 2024/04/06 14:54:40 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include "../includes/Server.hpp"
#include "../includes/ServerBlock.hpp"

int Server::socketCreate(ServerBlock &vSer)
{
	int sockfd;
	int on = 1;
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;		 // on hosts connected via an IPv4 network
	hints.ai_socktype = SOCK_STREAM; // use the TCP in order to communicate
	hints.ai_flags = AI_PASSIVE;	 //
	hints.ai_protocol = IPPROTO_TCP;

	std::cout << "Host : " << vSer.getHost() << " Port: " << vSer.getListen() << "\n";
	if (getaddrinfo((vSer.getHost()).c_str(), (vSer.getListen()).c_str(), &hints, &res) != 0)
		throw std::runtime_error("getaddrinfo failed\n");
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
		throw std::runtime_error("socket() failed\n");
	std::cout << "server: " << sockfd << " created\n";
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) // subject
		throw std::runtime_error("fcntl failed\n");
	// SO_REUSEADDR is used to enable the reusing of local addresses in the bind() function.

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)))
		throw std::runtime_error("setsockopt() failed\n");
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
	{
		throw std::runtime_error("bind failed");
	}
	if (listen(sockfd, 100) == -1)
	{
		throw std::runtime_error("lisen sys_call failed");
	}
	freeaddrinfo(res); // free the memory of the addrinfo
	vSer.setFdSocket(sockfd);
	return (sockfd);
}

void Server::addServersToEpoll()
{
	for (size_t i = 0; i < Vservers.size(); i++)
	{
		bzero(&event, sizeof(event));
		serverFd = socketCreate(Vservers[i]);

		event.data.fd = serverFd;
		event.events = EPOLLIN;
		if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event) == -1) // * add an fd to the intrest list
		{
			throw std::runtime_error("epoll_ctl [Server]");
		}
	}
}

// accept connection and add them to epoll

void Server::addCleintToEpoll(int index)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int fd = accept(Vservers[index].getFdSocket(), NULL, &clientAddrLen);
	if (fd < 0)
		throw std::runtime_error("accept\n");

	clients[fd] = new Client(fd, Vservers[index], files);
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLOUT;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw std::runtime_error("epoll_ctl [Cleint]");
	std::cout << YELLOW << "new Cleint added ,fd: " << fd << "\n"
			  << RESET;
}

bool Server::NewClient(int index)
{
	for (size_t j = 0; j < Vservers.size(); j++) // add the new Cleint to epoll
	{
		if (events[index].data.fd == Vservers[j].getFdSocket())
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
	}
	delete clients.find(ClientFd)->second;
	clients.erase(ClientFd);
	close(ClientFd);
	std::cout << "THE CLIENT DROPED\n";
}

// ! Desing Timeget.getResponse()
void Server::ServeClients(int index)
{
	bool tmp = 0;
	// this variable is to fix a chrome issue (sending two requests instead of one (when we try to get a video))
	// open the , which leads to  a fd leaks

	// ! in case of Bad Req or somthing that will not vist your reponse , you must setDoneServing here
	if (clients[events[index].data.fd]->getRequest()->getError() != 0)
	{
		clients[events[index].data.fd]->getResponseClass()->errorPage(clients[events[index].data.fd]->getRequest()->getError());
		clients[events[index].data.fd]->getRequest()->setDoneServing();
	}
	if (events[index].events & EPOLLIN)
	{
		// std::cout << "IN" << std::endl;
		if (clients[events[index].data.fd]->getRequest()->getDoneHeaderReading() == false)
			tmp = 1;
		clients[events[index].data.fd]->ReadParseReqHeader();
		if (clients[events[index].data.fd]->getRequest()->getMethdType() == POST) // ! i need to set donServing
		{
			clients[events[index].data.fd]->ServingClient();
			tmp = 1;
		}
		if (clients[events[index].data.fd]->getDoneServing())
		{
			write(this->clients[events[index].data.fd]->getSocketFd(),
				  this->clients[events[index].data.fd]->getResponseClass()->getResponse().c_str(),
				  this->clients[events[index].data.fd]->getResponseClass()->getResponse().size());
			tmp = 1;
		}
	}
	else if (events[index].events & EPOLLOUT)
	{
		std::cout << "HHHHHHHHHHHHHHHHHHHH\n";
		tmp = 1;
		if (clients[events[index].data.fd]->getRequest()->getMethdType() == DELETE)
		{
			clients[events[index].data.fd]->ServingClient();
		}
		if (clients[events[index].data.fd]->getRequest()->getMethdType() == GET)
		{
			clients[events[index].data.fd]->ServingClient();
		}
		if ((clients[events[index].data.fd]->getRequest()->getMethdType() == POST))
		{
			if (clients[events[index].data.fd]->getResponseClass()->postingDone)
				clients[events[index].data.fd]->ServingClient();
			else
				clients[events[index].data.fd]->getResponseClass()->timedOut();
		}
		write(this->clients[events[index].data.fd]->getSocketFd(),
			  this->clients[events[index].data.fd]->getResponseClass()->getResponse().c_str(),
			  this->clients[events[index].data.fd]->getResponseClass()->getResponse().size());
	}
	if (tmp == 0 && clients[events[index].data.fd]->getRequest()->getDoneHeaderReading())
	{
		clients[events[index].data.fd]->getRequest()->setDoneServing();
	}
}

int Server::ServerCore()
{
	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw std::runtime_error("epoll_create1 failed");
	addServersToEpoll();

	while (true)
	{
		int readyFd;
		bzero(events, sizeof(events));
		// std::cout << "***************** wiating for a new connection *******************\n";
		if ((readyFd = epoll_wait(epollFd, events, MAX_EVENTS, -1)) != 0)
		{
			if (readyFd < 0)
				throw std::runtime_error("epoll_wait error");
			for (int i = 0; i < readyFd; i++)
			{
				if (NewClient(i)) // add new client
				{
					std::cout << YELLOW << "new Cleint connected ,fd: " << events[i].data.fd << "\n"
							  << RESET;
				}
				else if (clients[events[i].data.fd]->getDoneServing() == false)
				{
					// std::cout << "Serving client\n";
					clients[events[i].data.fd]->getRequest()->timeOutCheching();
					ServeClients(i);
				}
				else
				{
					DropCleint(events[i].data.fd);
				}
			}
		}
	}
}

Server::Server(std::vector<ServerBlock> &Vser) : Vservers(Vser)
{
	files = getDataFromFiles();
}

Server::~Server()
{
}