/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/02 15:22:32 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# define BUF_SIZE 1024

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"
# include "../includes/Response.hpp"

class Request;
class Response;

class Client
{
private:
	int                 sockeFd;
	char                buf[BUF_SIZE];
	static std::string  Methods[3];
	int					MethodType;


//Pravite  Methodes
private:
	//void	Client::storeReqHeader(const std::string&);

// useless copy constructor and assingement operator
	Client(const Client&);
	Client& operator=(const Client&);
	Client();

public:
	Client(const VirtualServer*, int);
	~Client();

	bool    		DoneServing;
	bool    		DoneHeaderReading;
	const VirtualServer*	Vserver;

// Request and Respose classes
	Request		*request;
	Response	*response;
// parce the request
 	void	ReadParseReqHeader();
	void	ServingClient();

};




