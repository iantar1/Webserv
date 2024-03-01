/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 10:40:43 by iantar           ###   ########.fr       */
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
	Client(VirtualServer*);
	~Client();

	bool    DoneServing;
	bool    DoneHeaderReading;

// Request and Respose classes
	Request		request;
	Response	response;
// parce the request
 void   ReadReqHeader();

};

Client::Client()
{
}

Client::Client(int fd): sockeFd(fd)
{
}

Client::~Client()
{
}
