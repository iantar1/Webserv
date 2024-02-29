/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:27 by iantar            #+#    #+#             */
/*   Updated: 2024/02/29 11:10:09 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

calss Location;

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"

class VirtualServer
{
private:
	// listen;
	// server_name;
	// max_client_body_size;
	// error_page;
	
	int									port;
	std::string							Name;
	std::map<std::string, Location *>	locations;

	int					fdSocket;

public:
	VirtualServer();
	~VirtualServer();

	void	setFdSocket(int);
	int		getFdSocket() const;

public://Methods
	
};

int		VirtualServer::getFdSocket() const
{
	return (fdSocket);
}

void	VirtualServer::setFdSocket(int fd)
{
	fdSocket = fd;
}

VirtualServer::VirtualServer()
{
}

VirtualServer::~VirtualServer()
{
}
