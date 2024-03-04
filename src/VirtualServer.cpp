/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:29 by iantar            #+#    #+#             */
/*   Updated: 2024/03/04 17:58:53 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/VirtualServer.hpp"

int		VirtualServer::getFdSocket() const
{
	return (fdSocket);
}

void	VirtualServer::setFdSocket(int fd)
{
	fdSocket = fd;
}

const std::string&	VirtualServer::getVServerName() const
{
	return (Name);
}

in_addr_t			VirtualServer::getHostAddress() const
{
	return (HostAddress);
}

int					VirtualServer::getPort() const
{
	return (Port);
}
	
VirtualServer::VirtualServer()
{
}

VirtualServer::~VirtualServer()
{
}
