/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:03:11 by iantar            #+#    #+#             */
/*   Updated: 2024/03/06 20:45:18 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"

std::string Request::Methods[] = {"POST", "GET", "DELETE"};


Request::Request(int fd, VirtualServer *_Vserver) : SocketFd(fd), errorFlag(0), reading_done(0), Vserver(_Vserver) 
{
	MethodType = 0;
	std::cout << RED << "Requset Constructred\n" << RESET;
}


Request::~Request()
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

bool	Request::URI_ValidLength(const std::string& uri) const
{
	return (uri.size() > MAX_URI_CHAR_SIZE);
}

bool	Request::URI_ValidChar(const std::string& uri) const
{
	std::string	str = "-_.~";
	int	checker;

	for (size_t i = 0; i < uri.size(); i++)
	{
		checker = 0;
		if (std::isdigit(uri[i]) == false)
			checker++;
		if (std::isalpha(uri[i]) == false)
			checker++;
		if (str.find(uri[i]) == std::string::npos)
			checker++;
		if (checker == 3)
			return (1);
	}
	return (0);
}


bool	Request::URI_ValidLocation(const std::string& uri) const
{
	mapIterType	it = Vserver->getLocationsIterMap();

	
}

void	Request::URI_Checking(const std::string& uri)
{
	if (URI_ValidChar(uri))
	{
		ErrorFlag = BAD_REQ;
		return ;
	}
	if (URI_ValidLength(uri))
	{
		ErrorFlag = REQ_URI_TOO_LONG;
		return ;	
	}
	if (URI_ValidLocation)
	{
		ErrorFlag = NOT_FOUND;
		return ;
	}
}

void	Request::storeRequestLine(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;


	while (sstr >> word)
	{
		RequestLine.push_back(word);
	}
	URI_Checking(RequestLine[1]);
	oldPath = RequestLine[1]; // /hello.htmn for example
	SetNewPath(); // set new Path
	for (int i = 0; i < 3; i++)
	{
		if (Methods[i].compare(RequestLine[0]) == 0)
		{
			MethodType = i + 1;
			std::cout << "Req." << MethodType << " "<< RequestLine[0] << Methods[1] <<  ".\n";
			// std::cout << "CONSTRUCTOR\n";
			return ;
		}
	}
	throw std::runtime_error("Invalid Method");
}

void	Request::storeData(const std::string& dataRequest)
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
		}
		else
		{
			storeHeader(line);
		}
	}
    reading_done = true;
}


void	Request::SetNewPath()
{
	newPath = oldPath + Vserver->getRootLocatin(oldPath);
}


// ************ Getters **************

int Request::getMethdType() const
{
	return (this->MethodType);
}

int	Request::getFdSocket() const
{
	return (SocketFd);
}

int	Request::getTransferMode() const
{
	return (TransferMode);
}

int	Request::getError(void) const
{
	return this->ErrorCode;
}

std::string	Request::getBody(void) const
{
	return this->body;
}

std::string	Request::getChunkedBodySize(void) const
{
	return this->chunkedBodySize;
}

const std::string&  Request::getOldPath() const
{
	return (oldPath);
}

const std::string&  Request::getNewPath() const
{
	return (newPath);
}

// ************** Main Method *******************



void	Request::ParseRequest()
{
	int bytesRead;

    bytesRead = read(SocketFd, buf, BUF_SIZE);
    if (bytesRead < 0)
	{
        throw std::runtime_error("read syscall failed");
	}
	for (int i = 0; i < bytesRead; i++)
	{
		if (i + 3 < bytesRead && !strncmp(&(*(buf + i)), "\r\n\r\n", 4))	
			break ;
		HeaderReq += buf[i];
	}

	storeData(HeaderReq);
    reading_done = true;
}