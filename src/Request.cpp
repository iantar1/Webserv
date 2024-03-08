/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:03:11 by iantar            #+#    #+#             */
/*   Updated: 2024/03/08 18:52:46 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"

std::string Request::Methods[] = {"POST", "GET", "DELETE"};
std::string Request::validChars = "-._~:/?#[]@!$&'()*+,;=%";

Request::Request(int fd, VirtualServer *_Vserver) : SocketFd(fd), ErrorFlag(0), reading_done(0),
							Vserver(_Vserver), doneServing(false), doneReading(false), headerDone(false)
{
	MethodType = 0;
	FirstChunckBodySize = 0;
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

// *************** Private Error Chicking Methods *******************

void	Request::checkValidHeader()
{
	
}

void	Request::checkValid_GET_Header()
{
	if (MethodType == 0)
	{
		ErrorFlag = NOT_IMPLEMENTED;
		throw std::runtime_error("Not Implemented");
	}
	if (body.empty() == false)
	{
		ErrorFlag = BAD_REQ;
		throw std::runtime_error("Bad req");
	}
}

void	Request::checkValid_POST_Header()
{
	if (Header.find("Transfer-Encoding") != Header.end()
		&& Header["Transfer-Encoding"] != "chunked")
	{
		ErrorFlag = NOT_IMPLEMENTED;
		throw std::runtime_error("Not Implemented");
	}
	if (Header.find("Transfer-Encoding") == Header.end()
		&& Header.find("Content-Length") == Header.end())
	{
		ErrorFlag = BAD_REQ;
		throw std::runtime_error("bad Request");
	}
	if (Header.find("Content-Length") != Header.end())
	{
		if (atol((Header["Content-Length"]).c_str()) > )// !
	}
}

void	Request::checkValid_DELETE_Header()
{
	
}

void	Request::checkValidMethod()
{
	switch (getMethdType())
	{
	case GET:
		checkValid_GET_Header();
		break;
	case POST:
		checkValid_POST_Header();
		break;
	case DELETE:
		checkValid_DELETE_Header();
		break;
	default:
		
		break;
	}
}

bool	Request::URI_ValidLength(const std::string& uri) const
{
	return (uri.size() > MAX_URI_CHAR_SIZE);
}

bool	Request::URI_ValidChar(const std::string& uri) const
{
	for (size_t i = 0; i < uri.size(); i++)
	{
		if (!std::isdigit(uri[i]) && !std::isalpha(uri[i])
			&& validChars.find(uri[i]) == std::string::npos)
		{
			return (1);
		}
	}
	return (0);
}

bool	Request::URI_ValidLocation(const std::string& uri) const
{
	mapIterType	it = Vserver->getLocationsBeginIterMap();
	mapIterType	it_end = Vserver->getLocationsEndIterMap();

	for (;it != it_end; ++it)
	{
		if (uri.compare(0, (it->first).size(), it->first))
			return (1);
		if ((it->first).size() < uri.size() && uri[(it->first).size()] != '/')
			return (1);
	}
	return (0);
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
	if (URI_ValidLocation(uri))
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
	newPath = Vserver->getRootLocatin(oldPath) + oldPath;
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

int*	Request::getTransferMode()
{
	return (&TransferMode);
}

int	Request::getError(void) const
{
	return this->ErrorFlag;
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


bool	Request::getDoneServing() const
{
	return (this->doneServing);
}

bool	Request::getDoneReading() const
{
	return (this->doneServing);
}






// ************* Setters *************

void	Request::setDoneServing()
{
	this->doneServing = true;
}

void    Request::setDoneReading()
{
	this->doneReading = true;
}

void    Request::setLocation_str(std::string _location_str)
{
	this->location_str = _location_str;
}


// ************** Main Method *******************

bool	Request::ReadCheckHeader()
{
    bytesRead = read(SocketFd, buf, BUF_SIZE);
    if (bytesRead < 0)
	{
		ErrorFlag = INTERNAL_ERR;
        throw std::runtime_error("read syscall failed");
	}
	for (int i = 0; i < bytesRead; i++)
	{
		if (i + 3 < bytesRead && !strncmp(&(*(buf + i)), "\r\n\r\n", 4))
		{
			headerDone = true;
			storeData(HeaderReq);
			FirstChunckBodySize = bytesRead - (i + 4);
			return (true);
		}
		HeaderReq += buf[i];
	}
	return (false);
}

void	Request::saveFirstChuckBody()
{
	body = std::string(&(*(buf + FirstChunckBodySize - 1)), FirstChunckBodySize);
}


void	Request::ParseRequest()
{
	try
	{
		if (ReadCheckHeader())// * throw if error, return true if done
		{
			checkValidMethod(); // ! throw if error
			if (MethodType == POST && FirstChunckBodySize) // * save the first chunck body
				saveFirstChuckBody();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

// ************ Debug *****************

void	Request::printRequest()
{
	std::cout << "||************REQUEST************||\n";
	std::cout << RequestHeader << "\n";
	for (std::map<std::string, std::string>::iterator it = Header.begin(); it != Header.end(); ++it)
	{
		std::cout << it->second << "\n";
	}
}