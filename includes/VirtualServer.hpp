/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:27 by iantar            #+#    #+#             */
/*   Updated: 2024/03/04 18:20:29 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/Location.hpp"

class Location;

class VirtualServer
{
private:


	int									fdSocket;
	std::string							ServerName;
	int									Port;
	in_addr_t							HostAddress;
	size_t								maxCleintBodySize;

	std::map<std::string, Location *>	locations;

public:
	VirtualServer();
	// VirtualServer();
	~VirtualServer();

	void	setFdSocket(int);

// getters
	int					getFdSocket() const;
	const std::string&	getVServerName() const;
	in_addr_t			getHostAddress() const;
	int					getPort() const;
	
	void	SetLocation(Location*, std::string);
	//void	setVserverINfo(const std::string& name, );
public://Methods
	
};

void	VirtualServer::SetLocation(Location* Objloca, std::string strLocat)
{
	locations[strLocat] = Objloca;
}