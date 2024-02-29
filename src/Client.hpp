/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/02/28 15:17:45 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# define BUF_SIZE 1024

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"

class Client
{
private:
	int                 sockeFd;
	char                buf[BUF_SIZE];
	static std::string  Methods[3];
	int					MethodType;
	std::vector         RequestLine;

//Pravite  Methodes
private:
	void	Client::storeReqHeader(const std::string&);

	Client();
// useless copy constructor and assingement operator
	Client(const Client&);
	Client& operator=(const Client&);

public:
	Client(int fd);
	~Client();

	bool    DoneServing;
	bool    DoneHeaderReading;

// Request and Respose classes
	Request		request;
	Response	response;
// parce the request
 void   ReadReqHeader();

};

std::string parseRequest::Methods[] = {"POST", "GET", "DELETE"};

void    Client::ReadReqHeader()
{
   int  ReadSize;

	ReadSize = read(sockeFd, buf, BUF_SIZE);
	if (ReadSize == -1)
		throw std::runtime_error("read sys call failed!");
	storeReqHeader(std::string(buf));
	if (MethodType == POST)
	{
		// check if it is valid
		// store a a chunck from the previous
		
	}
}


void	Client::storeReqHeader(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;

	while (sstr >> word)
	{
		RequestLine.push_back(word);
	}
	for (int i = 0; i < 3; i++)
	{
		if (Methods[i] == RequestLine[0]) // check if it is allowed in this location
		{
			MethodType = i + 1;
			return ;
		}
	}
	throw std::runtime_error("Invalid Method");
}

Client::Client()
{
}

Client::Client(int fd): sockeFd(fd), DoneReading(false)
{
}

Client::~Client()
{
}
