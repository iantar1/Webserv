/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by iantar            #+#    #+#             */
/*   Updated: 2024/03/04 22:28:09 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/VirtualServer.hpp"

class VirtualServer;
class Client;

# define MAX_EVENTS 100
class Server
{
private:	
	int									serverFd;
	std::map<int, Client*>				clients;
	const std::vector<VirtualServer*>&  		Vservers;
	struct sockaddr_in					S_Addr;

	int domain;
	int type;
	int protocol;
	int epollFd;
// epoll structure
	struct epoll_event  event;
	struct epoll_event  events[100];

private:
	int		socketCreate(VirtualServer* vSer);
	void    addServersToEpoll();
	void    addCleintToEpoll(int);

private:
	Server& operator=(const Server&);
	Server(const Server&);
	Server();

public:
	Server(const std::vector<VirtualServer*>&);
	~Server();

	int launchServer();    
};

