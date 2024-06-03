/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:12:09 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 19:16:11 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include "../includes/Server.hpp"
#include "../includes/ServerBlock.hpp"

int Server::socketCreate(ServerBlock& vSer)
{
	int sockfd;
	int on = 1;
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;		 // on hosts connected via an IPv4 network
	hints.ai_socktype = SOCK_STREAM; // use the TCP in order to communicate
	hints.ai_flags = AI_PASSIVE;	 //
	hints.ai_protocol = IPPROTO_TCP;

	std::cout << "Serving HTTP on : " << vSer.getHost() << " Port: " << vSer.getListen() << " .....\n";
	if (getaddrinfo((vSer.getHost()).c_str(), (vSer.getListen()).c_str(), &hints, &res) != 0)
		throw std::runtime_error("getaddrinfo failed\n");
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		freeaddrinfo(res);
		throw std::runtime_error("socket() failed\n");
	}

	//! the subject : fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC); and only for MACOS
	// if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) // subject
	// 	throw std::runtime_error("fcntl failed\n");
	// SO_REUSEADDR is used to enable the reusing of local addresses in the bind() function.

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)))
	{
		freeaddrinfo(res);
		throw std::runtime_error("setsockopt() failed\n");
	}
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0)
	{
		freeaddrinfo(res);
		throw std::runtime_error("bind failed");
	}
	freeaddrinfo(res); // free the memory of the addrinfo
	if (listen(sockfd, MAX_EVENTS) == -1)// !dir xi macro hna lah ihdik
	{
		throw std::runtime_error("lisen sys_call failed");
	}
	vSer.setFdSocket(sockfd);
	return (sockfd);
}

void Server::addServersToEpoll()
{
	for (size_t i = 0; i < Vservers.size(); i++)
	{
		bzero(&event, sizeof(event));
		serverFd = socketCreate(Vservers[i]);
	//! the subject : fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC); and only for MACOS
		// if (fcntl(serverFd, F_SETFL, O_NONBLOCK) == -1)
		// 	throw std::runtime_error("fcntl failed\n");
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
	int fd;

	bzero(&clientAddr, sizeof(clientAddr));

	fd = accept(Vservers[index].getFdSocket(), NULL, &clientAddrLen);
	if (fd < 0)
	{
		std::cerr << "accept sys call failed\n";
		return;
	}
//! the subject : fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC); and only for MACOS
	// if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	// {
	// 	close(fd);
	// 	std::cerr << "fcntl failed\n";
	// 	return;
	// }
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		close(fd);
		std::cerr << "epoll_ctl failed\n";
		return;
	}
//TODO: u should add a set Vservrs function, if it does not exist
	clients[fd] = new Client(fd, Vservers[index], files, VservSimilars);
}

//* add the new Cleint to epoll
bool Server::NewClient(int index)
{
	for (size_t j = 0; j < Vservers.size(); j++)
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
	std::cout << "client dropped\n";
}

bool	Server::ep_OUT_handling(int index)
{
	int writed_data;

//* I added this when I changed alarm() to kill()
	clients[events[index].data.fd]->killChild();//! awddiii
	if (clients[events[index].data.fd]->getRequest()->getError())
	{
		clients[events[index].data.fd]->response->errorPage(clients[events[index].data.fd]->getRequest()->getError());
		clients[events[index].data.fd]->getRequest()->setDoneServing();
	}
	//* if the method is GET  geti and check if you done or somthing went wrong
	else if (clients[events[index].data.fd]->getRequest()->getMethdType() == GET
		&& !clients[events[index].data.fd]->getRequest()->getDoneServing())
	{
		// !don't repeate yourself
		if (!clients[events[index].data.fd]->response->prePre())//! what ?
		{
			if (clients[events[index].data.fd]->response->indexCgiCheck())
			{
				if (!clients[events[index].data.fd]->response->constructedCgiHeader)
					clients[events[index].data.fd]->response->cgiResponse();
				if (clients[events[index].data.fd]->response->cgi_Handler(clients[events[index].data.fd]->response->cgiIndexPath) == false)// not done
				{
					return (1);
				}
				else if (clients[events[index].data.fd]->getRequest()->getError())
				{
					clients[events[index].data.fd]->response->errorPage(clients[events[index].data.fd]->getRequest()->getError());
					clients[events[index].data.fd]->getRequest()->setDoneServing();

				}
			}
			else if (clients[events[index].data.fd]->response->isCGI())
			{
				if (!clients[events[index].data.fd]->response->constructedCgiHeader)
				{
					clients[events[index].data.fd]->response->cgiResponse();	
				}
				if (clients[events[index].data.fd]->response->cgi_Handler() == false)// not done
				{
					return (1);
				}
				else if (clients[events[index].data.fd]->getRequest()->getError())
				{
					clients[events[index].data.fd]->response->errorPage(clients[events[index].data.fd]->getRequest()->getError());
					clients[events[index].data.fd]->getRequest()->setDoneServing();
				}
			}
			else
			{
				clients[events[index].data.fd]->response->theGetMethod();
			}
		}
	}
	else if (clients[events[index].data.fd]->getRequest()->getMethdType() == DELETE)
	{
		// * i put here just to not add a condition of doneHeaderReading
		clients[events[index].data.fd]->response->DeleteMethod();
	}
	//* if the method is POST or DELETE -> response and dropClient
	else if (clients[events[index].data.fd]->getRequest()->getMethdType() == POST)
	{
	//! here it was isDonServing, but it's not what should be , you will never done if you don't fill the response
	//! and whemn youy done posting , you should response
		if (clients[events[index].data.fd]->response->postingDone == false)
		{
			this->clients[events[index].data.fd]->response->timedOut();
			return (1);
		}
	// what is this Nooh
		if (clients[events[index].data.fd]->response->isCGI()
			&& clients[events[index].data.fd]->response->cgi_Handler() == false)// not done yet
		{
			return (1);
		}
		if (!clients[events[index].data.fd]->response->constructedCgiHeader)
		{
			clients[events[index].data.fd]->response->cgiResponse();
		}
		clients[events[index].data.fd]->getResponseClass()->fillResponse();
	}
	if ((writed_data = write(this->clients[events[index].data.fd]->getSocketFd(),
		this->clients[events[index].data.fd]->getResponseClass()->getResponse().c_str(),
		this->clients[events[index].data.fd]->getResponseClass()->getResponse().size())) != 0)
	{
		if (clients[events[index].data.fd]->getRequest()->getDoneServing() || writed_data < 0)
		{
			DropCleint(events[index].data.fd);//! jhdcjhdsjk d
		}
		return (1);
	}
	return (0);
}

bool	Server::ep_IN_handling(int index)
{
	// *accept new connection,
	if (NewClient(index))
	{
		return (1);
	}
//* I added this when I changed alarm() to kill() sysCall
	clients[events[index].data.fd]->killChild();

	clients[events[index].data.fd]->ReadParseReqHeader();

	if (clients[events[index].data.fd]->request.getError())
	{
		if (clients[events[index].data.fd]->request.getError() == READ_FAILED)
			DropCleint(events[index].data.fd);
		return (1);
	}

//* if we don't finish reading the request return false, to check the RequestTimout
	if (clients[events[index].data.fd]->request.getDoneHeaderReading() == false)
	{
		return (0);
	}
	clients[events[index].data.fd]->response->setURI();//! khss hadxi ithyid mn hna mn b3d (ugly)

//* if it is POST and  the is not an error relited to the request, then call post	
	if (clients[events[index].data.fd]->getRequest()->getMethdType() == POST)
	{
		clients[events[index].data.fd]->response->posting();
	}
	return (0);
}

void Server::ServerCore()
{
	int readyFd;

	epollFd = epoll_create1(0);
	if (epollFd < 0)
		throw std::runtime_error("epoll_create1 failed");

	addServersToEpoll();

	while (true)
	{
		bzero(events, sizeof(events));
		if ((readyFd = epoll_wait(epollFd, events, MAX_EVENTS, -1)) != 0)
		{
			if (readyFd < 0)
				throw std::runtime_error("epoll_wait error");
			for (int i = 0; i < readyFd; i++)
			{
				if (events[i].events & EPOLLERR || events[i].events & EPOLLRDHUP || events[i].events & EPOLLHUP)
				{
					DropCleint(events[i].data.fd);
					continue ;
				}
				if (events[i].events & EPOLLIN)
				{
					if (ep_IN_handling(i))
						continue;
				}
				else if (events[i].events & EPOLLOUT)
				{
					if (ep_OUT_handling(i))
					{
						continue;
					}
				}
				clients[events[i].data.fd]->timeOutChecking();
			}
		}
	}
}

Server::Server(std::vector<ServerBlock>& Vser, std::map<int, std::vector<ServerBlock> > VserSimilars) 
		: Vservers(Vser), VservSimilars(VserSimilars)
{
	files = getDataFromFiles();
}

Server::~Server()
{
}
