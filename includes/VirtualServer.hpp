/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:27 by iantar            #+#    #+#             */
/*   Updated: 2024/03/17 05:41:25 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/Location.hpp"

class Location;

typedef	std::map<std::string, Location *>::const_iterator	mapIterType;

class VirtualServer
{
private:

	int									fdSocket;
	std::string							ServerName;
	size_t								maxCleintBodySize;


public:
// public Attrebiute (class)
	std::map<std::string, Location *>	locations;

	std::string							Port;
	std::string							HostAddress;
	VirtualServer();
	// VirtualServer();
	~VirtualServer();

	void	setFdSocket(int);

// getters
	int					getFdSocket() const;
	const std::string&	getVServerName() const;
	const std::string&	getPort() const;
	const std::string&	getHost() const;
	const std::string&	getRootLocatin(const std::string&);
	mapIterType			getLocationsBeginIterMap() const;
	mapIterType			getLocationsEndIterMap() const;
	const std::string&	getLocation(std::string);
	
	void	SetLocation(Location*, std::string);
	// void	setVserverINfo(const std::string& name, );
public://Methods
	
	
};
