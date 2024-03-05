/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:29 by iantar            #+#    #+#             */
/*   Updated: 2024/03/05 18:02:32 by iantar           ###   ########.fr       */
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

// const std::string&	VirtualServer::getVServerName() const
// {
// 	return (N);
// }

void	VirtualServer::SetLocation(Location* loc, std::string strLocation)
{
	locations[strLocation] = loc;
}

const std::string&	VirtualServer::getHost() const
{
	return (HostAddress);
}

const std::string&	VirtualServer::getPort() const
{
	return (Port);
}
	
VirtualServer::VirtualServer()
{
}

VirtualServer::~VirtualServer()
{
}

const std::string& VirtualServer::getRootLocatin(const std::string& _locStr)
{
	return (locations[_locStr]->getRoot());
}
