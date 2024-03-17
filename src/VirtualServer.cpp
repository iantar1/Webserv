/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:29 by iantar            #+#    #+#             */
/*   Updated: 2024/03/17 05:41:30 by nabboune         ###   ########.fr       */
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
	mapIterType	 it = getLocationsBeginIterMap();
	mapIterType	 it_end = getLocationsEndIterMap();

	for (;it != it_end;++it)
	{
		if (_locStr.compare(0, (it->first).size(), it->first) == 0)
		{
			break ;
		}
	}
	return (locations[it->first]->getRoot());
}

mapIterType	VirtualServer::getLocationsBeginIterMap() const
{
	return (locations.begin());
}

mapIterType	VirtualServer::getLocationsEndIterMap() const
{
	return (locations.end());
}

const std::string&	VirtualServer::getLocation(std::string location_str)
{
	return ((locations[location_str])->getLocation());
}
