/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:30:58 by iantar            #+#    #+#             */
/*   Updated: 2024/02/29 10:32:06 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# define BAD_REQ 400 
# define NOT_IMPLEMENTED 501
# define REQ_URI_TOO_LONG 414
# define REQ_ENTITY_TOO_LONG 413  

std::string parseRequest::Methods[] = {"POST", "GET", "DELETE"};

class Request
{
private:
    int                                 SocketFd;
    int                                 bytesRead;
    int                                 errorFlag;
    std::string                         RequestHeader;
    static std::string					Methods[3];
    std::map<std::string, std::string>	Header;
	int									MethodType;

// useless Constructor , copy constructor and assingment operator
    Request();
    Request(const Request&);
    Request& operator=(const Request&);

public:
    Request(int);
    ~Request();
    void    readHeader() const;
    void    parceRequest();
};

void    Request::storeHeader()
{

}

void	Request::storeHeader(const std::string& line)
{
	std::string	key;
	std::string	value;
	size_t		index;

	index = line.find(":");
	key = line.substr(0, index + 1);
	value = line.substr(index + 1);
	Header[key] = value;
}

void	Request::storeRequestLine(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;

	while (sstr >> word)
	{
		RequestLine.push_back(word);
	}
	for (int i = 0; i < 3; i++)
	{
		if (Methods[i] == RequestLine[0])
		{
			MethodType = i + 1;
			return ;
		}
	}
	throw std::runtime_error("Invalid Method");
}

void    Request::readHeader() const
{
    std::istringstream iss(dataRequest);
    std::string line;

    bytesRead = read(clientSocket, buf, BUF_SIZE);
    if (bytesRead < 0)
        throw std::runtime_error("Request: read syscall failed");
    RequestHeader.append(buf, bytesRead);
    if ((index = data.find("\r\n\r\n")) != std::string::npos) {
	    storeData(data, index + 4); // if it POST you need to store what come after \r\n\r\n
        reading_done = true;
    }
	for (int i = 0; std::getline(iss, line); i++)
	{
		if (i == 0)
		{
			storeRequestLine(line);
		}
		else if (line == "\r")
		{
			break ;
		}
		else
		{
			storeHeader(line);
		}
	}
}

Request::Request(int fd) : SocketFd(fd), errorFlag(0)
{
}

Request::~Request()
{
}
