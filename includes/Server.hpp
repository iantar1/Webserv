/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:06 by iantar            #+#    #+#             */
/*   Updated: 2024/06/01 18:15:01 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/ServerBlock.hpp"
# include "../includes/Client.hpp"


# define MAX_EVENTS 100
class Server
{
private:	
	int											serverFd;
	std::map<int, Client*>						clients;
	std::vector<ServerBlock>&  					Vservers;
	std::map<int, std::vector<ServerBlock> >	VservSimilars;
	struct sockaddr_in							S_Addr;
		
	t_files										files;
	int 										domain;
	int 										type;
	int 										protocol;
	int 										epollFd;
// epoll structure		
	struct epoll_event  						event;
	struct epoll_event  						events[MAX_EVENTS];
	int											connectedClients;

//************ private Methods **************
private:
	int		socketCreate(ServerBlock& vSer);
	void	addServersToEpoll();
	void	addCleintToEpoll(int);
	bool	NewClient(int);
	void	DropCleint(int);
	bool	ep_OUT_handling(int);
	bool	ep_IN_handling(int);

private:
	Server& operator=(const Server&);
	Server(const Server&);
	Server();

public:
	Server(std::vector<ServerBlock>&, std::map<int, std::vector<ServerBlock> >);
	~Server();

	void ServerCore(); 
};

