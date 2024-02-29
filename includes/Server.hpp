/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by iantar            #+#    #+#             */
/*   Updated: 2024/02/29 14:58:15 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"

class VirtualServer;

# define MAX_EVENTS 100
class Server
{
private:	
	int									serverFd;
	std::map<int, Client>				clients;
	const std::vector<VirtualServer*>&  Vservers;

	int domain;
	int type;
	int protocol;
	int epollFd;
// epoll structure
	struct epoll_event  event;
	struct epoll_event  events[100];
	int					epollFd;

private:
	int		socketCreate(const VirtualServer& vSer);
	void    addServersToEpoll();
	void    addCleintToEpoll();

private:
	Server& operator=(const Server&);
	Server(const Server&);

public:
	Server();
	~Server();

	int launchServer();    
};

