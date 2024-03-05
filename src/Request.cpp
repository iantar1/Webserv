/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:03:11 by iantar            #+#    #+#             */
/*   Updated: 2024/03/05 16:45:00 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"

std::string Request::Methods[] = {"POST", "GET", "DELETE"};


Request::Request(int fd, VirtualServer *_Vserver) : SocketFd(fd), errorFlag(0), reading_done(0), Vserver(_Vserver) 
{
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

void	Request::storeRequestLine(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;

	while (sstr >> word)
	{
		RequestLine.push_back(word);
	}
	oldPath = RequestLine[1]; // /hello.htmn for example
	SetNewPath(); // set new Path
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

void	Request::storeData(const std::string& dataRequest, size_t index)
{
	std::istringstream iss(dataRequest);
    std::string line;

    (void)index;
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

// main Method

void	Request::SetNewPath()
{
	newPath = oldPath + Vserver->getRootLocatin(oldPath);
}

void	Request::ParseRequest()
{
	int bytesRead;
    size_t index;
    std::string data;

    std::cout << "cleint : " << SocketFd << std::endl;
    bytesRead = read(SocketFd, buf, BUF_SIZE);
    if (bytesRead < 0)
        throw std::runtime_error("read syscall failed");
    data.append(buf, bytesRead);
    if ((index = data.find("\r\n\r\n")) != std::string::npos) {
	    storeData(data, index + 4);
        reading_done = true;
    }
	
}

// ************ Getters **************

int Request::getMethdType() const
{
	return (MethodType);
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
	return (newPath);
}

const std::string&  Request::getNewPath() const
{
	return (oldPath);
}
