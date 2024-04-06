/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:12:09 by iantar            #+#    #+#             */
/*   Updated: 2024/04/05 03:20:38 by iantar           ###   ########.fr       */
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

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;		 // on hosts connected via an IPv4 network
	hints.ai_socktype = SOCK_STREAM; // use the TCP in order to communicate
	hints.ai_flags = AI_PASSIVE;	 //
	hints.ai_protocol = IPPROTO_TCP;

	// ! replacing getaddrinfo by inet_addr
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

bool Server::addCleintToEpoll(int index)
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	int fd = accept(Vservers[index].getFdSocket(), NULL, &clientAddrLen);
	if (fd < 0)
		return 0;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLOUT;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) // subject
	{
		close(fd);
		return 0;
	}
	if (epoll_ctl(fd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		close(fd);
		return 0;
	}
	clients[fd] = new Client(fd, Vservers[index], files);
	std::cout << YELLOW << "new Cleint added ,fd: " << fd << "\n"
			  << RESET;
	return 1;
}

int Server::isServerSocket(int fd)
{
	for (size_t j = 0; j < Vservers.size(); j++) // add the new Cleint to epoll
	{
		if (fd == Vservers[j].getFdSocket())
			return (j);
	}
	return (-1);
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
// TODO I think doneServin it must be in the client

int Server::ServerCore()
{
	signal(SIGPIPE, SIG_IGN);
	int index = 0;

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
				if ((index = isServerSocket(events[i].data.fd)) > 0) // add new client
				{
					Server::addCleintToEpoll(index);
					std::cout << YELLOW << "new Cleint connected ,fd: " << events[i].data.fd << "\n"
							  << RESET;
				}
				else if (clients[events[i].data.fd]->getDoneServing() == false)
				{

					if (events[index].events & EPOLLIN)
					{
						clients[events[i].data.fd]->request.ReadRequest();
						if (clients[events[i].data.fd]->request.getMethdType() == POST)
							clients[events[i].data.fd]->response->post();
						// write(events[i].data.fd, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hi there</h1></body></html>", 81);
					}
					else if (events[index].events & EPOLLOUT)
					{
						std::cerr << "EPOLLOUT" << std::endl;
						write(events[i].data.fd, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hi there</h1></body></html>", 81);
						clients[events[i].data.fd]->setDoneServing();
					}
					if (clients[events[i].data.fd]->getDoneServing() == true)
					{
						std::cout << "THE CLIENT DONE SERVING\n";
						DropCleint(events[i].data.fd);
					}	
					// std::cout << "Serving client\n";
					// try
					// {
					// 	clients[events[i].data.fd]->getRequest()->timeOutCheching();
					// 	ServeClients(i);
					// }
					// catch (int code)
					// {
					// 	std::cerr << code << '\n';
					// }
					// catch(const std::exception& e)
					// {
					// 	std::cerr << e.what() << '\n';
					// }
				}
				else
				{
					// DropCleint(events[i].data.fd);
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