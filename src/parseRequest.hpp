/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:01:01 by iantar            #+#    #+#             */
/*   Updated: 2024/02/09 11:25:41 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "includes/headers.hpp"
# include "includes/macros.hpp"
# define BUF_SIZE 1024
# define COLON 58
# define CARRIGE_RET 13
# define POST 
# define GET 
# define DELETE 


class parseRequest
{
private:
	int 								Sockfd;
	int									MethodType;
	char								buf[BUF_SIZE];
	int									readSize;
	std::vector<std::string>			RequestLine;
	std::map<std::string, std::string>	Header;
	std::string							Body;

// private default constructor
	parseRequest();

// privatre methods

	void	storeData(const std::string&);
	void	storeHeader(const std::string&);
	void	storeRequestLine(const std::string&);

public:
// parametrize constructor, copy constructor, copy assignement operator and destructor
	parseRequest(int);
	parseRequest(const parseRequest&);
	parseRequest& operator=(const parseRequest&);
	~parseRequest();

	void	readData();
};

parseRequest::parseRequest(int sockfd) : Sockfd(sockfd), MethodType(0)
{
}

void	parseRequest::storeHeader(const std::string& line)
{
	std::string	key;
	std::string	value;
	size_t		index;

	index = line.find(":");
	key = line.substr(0, index + 1);
	value = line.substr(index + 1);
	Header[key] = value;
}

void	parseRequest::storeRequestLine(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;

	while (sstr >> word)
	{
		RequestLine.push_back(word);
	}
}

void	parseRequest::storeData(const std::string& dataRequest)
{
	std::istringstream iss(dataRequest);
    std::string line;

	for (int i = 0; std::getline(iss, line); i++)
	{
		if (i == 0)
		{
			storeRequestLine(line);
		}
		else if (line == "\r")
		{
			break ;
			//storeBody(line);
			//return ;
		}
		else
		{
			storeHeader(line);
		}
	}
	while (std::getline(iss, line)) // if there is a body , mabye you need to read again
	{
		Body += line;
	}
}

void	parseRequest::readData(void)
{
	readSize = read(Sockfd, buf, BUF_SIZE - 1);

	if (readSize == -1)
		throw std::runtime_error("can't read client socket");
	storeData(std::string(buf, readSize));
	
// if you don't spesifies the size to std::string's constructor, the string will stop at '\0', 
// whish will lead you to lose a part of your body
}

parseRequest::~parseRequest()
{
}
