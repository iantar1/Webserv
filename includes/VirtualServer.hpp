/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:09:27 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 21:35:54 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/Location.hpp"

class Location;
// vector<VirtualServer*>
class VirtualServer
{
private:
	// listen;
	// server_name;
	// max_client_body_size;
	// error_page;

	int									fdSocket;
	std::string							Name;
	int									Port;
	in_addr_t							HostAddress;
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
	
	//void	setVserverINfo(const std::string& name, );
public://Methods
	
};


