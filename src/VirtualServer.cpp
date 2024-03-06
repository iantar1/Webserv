/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:29 by iantar            #+#    #+#             */
/*   Updated: 2024/03/06 20:50:41 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/VirtualServer.hpp"

// ***************** Constructors , Destructors *****************

VirtualServer::VirtualServer()
{
}

VirtualServer::~VirtualServer()
{
}

// ********************** Setters **************************

void	VirtualServer::setFdSocket(int fd)
{
	fdSocket = fd;
}


void	VirtualServer::SetLocation(Location* loc, std::string strLocation)
{
	locations[strLocation] = loc;
}


// ***************** Getters *********************

// const std::string&	VirtualServer::getVServerName() const
// {
// 	return (N);
// }

int		VirtualServer::getFdSocket() const
{
	return (fdSocket);
}

const std::string&	VirtualServer::getHost() const
{
	return (HostAddress);
}

const std::string&	VirtualServer::getPort() const
{
	return (Port);
}
	

const std::string& VirtualServer::getRootLocatin(const std::string& _locStr)
{
	std::cout << "look\t" << _locStr << "\n";
	return (locations[_locStr]->getRoot());
}

mapIterType	VirtualServer::getLocationsIterMap() const
{
	return (locations.begin());
}

mapIterType	VirtualServer::getEndIterMap() const
{
	return (locations.end());
}
