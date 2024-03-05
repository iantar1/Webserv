/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/05 11:01:38 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# define BUF_SIZE 1024

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"
# include "../includes/Response.hpp"

# define POST 1
# define GET 2
# define DELETE 3 

class Request;
class Response;

class Client
{
private:
	int                 sockeFd;
	const VirtualServer*	Vserver;
	char                buf[BUF_SIZE];
	int					MethodType;
	Request				request;
	
	
	AMethod*			method;

	
//Pravite  Methodes
private:

	Client(const Client&);
	Client& operator=(const Client&);
	Client();

public:
	Client(const VirtualServer*, int);
	~Client();

// Request and Respose classes
	// std::map<std::string , AMethod*> respose;

	bool    		DoneServing;
	bool    		DoneHeaderReading;

 	void	ReadParseReqHeader();
	void	ServingClient();
	void	PrintRequest() const;

};



