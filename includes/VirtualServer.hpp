/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:27 by iantar            #+#    #+#             */
/*   Updated: 2024/03/05 18:15:36 by iantar           ###   ########.fr       */
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
	size_t								maxCleintBodySize;

	std::map<std::string, Location *>	locations;

public:
	std::string							Port;
	std::string							HostAddress;
	VirtualServer();
	// VirtualServer();
	~VirtualServer();

	void	setFdSocket(int);

// getters
	int					getFdSocket() const;
	const std::string&	getVServerName() const;
	const std::string& getPort() const;
	const std::string& getHost() const;
	const std::string& getRootLocatin(const std::string&);
	
	void	SetLocation(Location*, std::string);
	// void	setVserverINfo(const std::string& name, );
public://Methods
	
	
};
