/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:03:11 by iantar            #+#    #+#             */
/*   Updated: 2024/03/13 23:58:09 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Request.hpp"
# include "../includes/utils.hpp"

std::string Request::Methods[] = {"POST", "GET", "DELETE"};
std::string Request::validChars = "-._~:/?#[]@!$&'()*+,;=%";


Request::Request(int fd, VirtualServer *_Vserver) : Vserver(_Vserver),
		 SocketFd(fd), ErrorFlag(0),doneServing(false), doneHeaderReading(false)
{
	MethodType = 0;
	FirstChunckBodySize = 0;
}


Request::~Request()
{
}


// *************** Private Error Chicking Methods *******************

void	Request::checkValidHeader()
{
	
}

void	Request::checkValid_GET_Header()
{
	if (body.empty() == false)
	{
		setFlagError(BAD_REQ, "bad Request");
	}
	for (size_t i = 0 ; i < (location->allowedMethods).size(); i++)
	{
		if (location->allowedMethods[i].compare("GET") == 0)
			return ;
	}
	setFlagError(METHOD_NOT_ALLOWED, "Method not allwed");
}

void	Request::checkValid_POST_Header()
{

	// std::map<std::string, std::string>::iterator	it = this->Header.find("transfer-encoding");
	// std::cout << "*******************************************************" << std::endl;
	// while (it != this->Header.end()) {
		// std::cout << it->first << " || " << it->second << " || " << it->second.compare("chunked") << std::endl;
	// 	it++;
	// }
	// std::cout << "*******************************************************" << std::endl;



	if (this->Header.find("transfer-encoding") != this->Header.end()
		&& this->Header.find("transfer-encoding")->second.compare("chunked"))
	{
		std::cout << Header["transfer-encoding"] << " <==" <<std::endl;
		setFlagError(NOT_IMPLEMENTED, "Not Implemented");
	}
	if (Header.find("transfer-encoding") == Header.end()
		&& Header.find("content-length") == Header.end())
	{
		setFlagError(BAD_REQ, "bad Request");
	}
	if (Header.find("content-length") != Header.end())
	{
		if (atol((Header["content-length"]).c_str()) > location->getMaxBodySize())
		{
			setFlagError(REQ_ENTITY_TOO_LONG, "Request Entity Too Large");
		}
	}
}

void	Request::checkValid_DELETE_Header()
{
		if (body.empty() == false)
	{
		setFlagError(BAD_REQ, "bad Request");
	}
	for (size_t i = 0 ; i < (location->allowedMethods).size(); i++)
	{
		if (location->allowedMethods[i].compare("DELETE") == 0)
			return ;
	}
	setFlagError(METHOD_NOT_ALLOWED, "Method not allwed");
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

bool    Request::URI_ValidLocation(const std::string& uri)
{
    mapIterType    it = Vserver->getLocationsBeginIterMap();
    mapIterType    it_end = Vserver->getLocationsEndIterMap();

    for (;it != it_end; ++it)
    {
        if (uri.compare(0, (it->first).size(), it->first) == 0)
        {
			location_str = std::string(it->first);
			location = it->second;
            if ((it->first).size() == uri.size())
                return (0);
            if ((it->first).size() < uri.size() && uri[(it->first).size()] == '/')
                return (0);
			if (it->first == "/") // hard code
				return (0);
        }
    }
    return (1);
}

void	Request::URI_Checking(const std::string& uri)
{
	if (URI_ValidChar(uri))
	{
		setFlagError(BAD_REQ, "BAD_REQ");
	}
	if (URI_ValidLength(uri))
	{
		setFlagError(REQ_URI_TOO_LONG, "REQ_URI_TOO_LONG");	
	}
	if (URI_ValidLocation(uri))
	{
		setFlagError(NOT_FOUND, "NOT FOUND");
	}
}

void	Request::httpVersionCheck(const std::string& http)
{
	if (http.compare("HTTP/1.1"))
		setFlagError(BAD_REQ, "BAD REQUEST");
}

void	Request::WhichMethod(const std::string& method)
{
	for (int i = 0; i < 3; i++)
	{
		if (Methods[i].compare(method) == 0)
		{
			MethodType = i + 1;
			return ;
		}
	}
	setFlagError(NOT_IMPLEMENTED, "Invalid Method");
}

void	Request::storeRequestLine(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;


	for (int i = 0; sstr >> word; i++)
	{
		if (i == 3)
			setFlagError(BAD_REQ, "BAD REQUEST");
		RequestLine.push_back(word);
	}
	WhichMethod(RequestLine[0]);
	URI_Checking(RequestLine[1]);
	httpVersionCheck(RequestLine[2]);
	oldPath = RequestLine[1]; // /hello.htmn for example
	SetNewPath(); // set new Path
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

int	Request::getTransferMode()
{
	return (TransferMode);
}

int	Request::getError(void) const
{
	return this->ErrorFlag;
}

const std::string&	Request::getBody(void) const
{
	return this->body;
}

const std::string&	Request::getChunkedBodySize(void) const
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

bool	Request::getDoneHeaderReading() const
{
	return (this->doneHeaderReading);
}

const std::map<std::string, std::string>&  Request::getRequest() const
{
	return (this->Header);
}


// ************* Setters *************

void	Request::setDoneServing()
{
	this->doneServing = true;
}

void    Request::setDoneHeaderReading()
{
	this->doneHeaderReading = true;
}

void    Request::setLocation_str(std::string _location_str)
{
	this->location_str = _location_str;
}

void	Request::setFlagError(int error_flag, const std::string& mes)
{
	ErrorFlag = error_flag;
	throw std::runtime_error(mes);
}

std::string Request::skipLeadingWhitespace(const std::string& str)
{
    std::string result;
	size_t index;

    for (index = 0; index < str.length() && isspace(str[index]); ++index) {}
    return (str.substr(index));
}

void	Request::storeHeader(const std::string& line)
{
	std::string	key;
	std::string	value;
	size_t		index;

	index = line.find(":");
	if (index == std::string::npos)
		setFlagError(BAD_REQ, "bad Request");
	key = toLower(line.substr(0, index));
	value = skipLeadingWhitespace(line.substr(index));
	Header[key] = value;
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
    doneHeaderReading = true;
}


void	Request::saveFirstChuckBody()
{
	body = std::string(buf + bytesRead - FirstChunckBodySize, FirstChunckBodySize);
}

void	Request::storeBody()
{
	body.clear();
	if (FirstChunckBodySize)
	{
		saveFirstChuckBody();
		FirstChunckBodySize = 0;
		return ;
	}
	body = std::string(buf, bytesRead);
}

// ************** Main Methods *******************

bool	Request::ReadCheckHeader()
{
	for (int i = 0; i < bytesRead; i++)
	{
		if (i + 3 < bytesRead && !strncmp(buf + i, "\r\n\r\n", 4))
		{
			doneHeaderReading = true;
			storeData(HeaderReq);
			FirstChunckBodySize = bytesRead - (i + 4);

			return (true);
		}
		HeaderReq += buf[i];
	}
	return (false);
}

void	Request::ReadRequest()
{
	try
	{
		bytesRead = read(SocketFd, buf, BUF_SIZE);

		// std::cout << buf << std::endl;
		// std::cout << "================== END =================" << std::endl;
		std::cout << "BytesRead = " << bytesRead << std::endl;

		if (bytesRead < 0)
			std::runtime_error("read system call failed\n");// ! should i set a flag ?
		if (bytesRead == 0 && !doneHeaderReading)
			setFlagError(BAD_REQ, "thre is No \\r\\n\\r\\n");

		if (ReadCheckHeader())
		{
			checkValidMethod();
		}
		if (doneHeaderReading)
		{
			if (MethodType == POST) // * save the first chunck body
			{

				storeBody();
			}
		}
		printRequest();
		
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RESET << '\n';
	}
}

// ************ Debug *****************

void	Request::printRequest()
{
	std::cout << "||************REQUEST HEADER************||\n";
	//std::cout << RequestHeader << "\n";
	for (std::map<std::string, std::string>::iterator it = Header.begin(); it != Header.end(); ++it)
	{
		std::cout << it->first << ":"<< it->second << "\n";
	}
	std::cout << "||************REQUEST Body************||\n\n";
	std::cout << body << "\n";
	std::cout << "||************************************||\n";
	
}

