/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/06 10:39:36 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# define BUF_SIZE 1024

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"
# include "../includes/Response.hpp"
# include "../includes/VirtualServer.hpp"


# define POST 1
# define GET 2
# define DELETE 3 

// class Request;
// class Response;

class Client
{
private:
	int                 sockeFd;
	VirtualServer*		Vserver;
	t_files*				files;
	char                buf[BUF_SIZE];
	int					MethodType;
	Request				request;
	Response			*response;

	
	// AMethod*			method;

	
//Pravite  Methodes
private:

	Client(const Client&);
	Client& operator=(const Client&);
	Client();

public:
	Client(int, VirtualServer*, t_files*);
	~Client();

// Request and Respose classes
	// std::map<std::string , AMethod*> respose;

	bool    		DoneServing;
	bool    		DoneHeaderReading;

 	void	ReadParseReqHeader();
	void	ServingClient();
	void	PrintRequest() const;

};



