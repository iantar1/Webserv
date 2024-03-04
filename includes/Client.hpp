/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/04 17:37:04 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# define BUF_SIZE 1024

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"
# include "../includes/Response.hpp"
// # include "../nabboneWork/includes/Request.hpp"


class Request;
class Response;

class Client
{
private:
	int                 sockeFd;
	const VirtualServer*	Vserver;
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

// Request and Respose classes
	// std::map<std::string , AMethod*> respose;

	bool    		DoneServing;
	bool    		DoneHeaderReading;
	Request		request;
// parce the request
 	void	ReadParseReqHeader();
	void	ServingClient();

};




// 	allowed_methods POST GET
// 	max_body_size 25000000
// 	upload_post /Uploads/
// 	upload_path /Uploads/
// 	error_page 404 defaultPages/400.htm
// 	autoindex on
// redirect 301 /Media/op.png /Media/op.gif
// redirect 302 /Media/op.mp4 /Media/op.gif
// redirect 301 /Media/op.jpeg /Media/op.gif
// 	index defaultPages/201.htm
// 	root /nfs/homes/nabboune/Desktop/Projects/WebServ/
// 	cgi .py /usr/bin/python3

