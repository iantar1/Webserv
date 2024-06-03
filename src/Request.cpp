/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:03:11 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 19:18:18 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

std::string Request::Methods[] = { "POST", "GET", "DELETE", "HEAD", "PUT", "CONNECT", "OPTIONS", "TRACE" };
std::string Request::validChars = "-._~:/?#[]@!$&'()*+,;=%";

Request::Request(int fd, ServerBlock _Vserver, std::map<int, std::vector<ServerBlock> > _VserSimilars) : SocketFd(fd), ErrorFlag(0),
doneServing(false), doneHeaderReading(false), checked(false), Vserver(_Vserver)
{
	MethodType = 0;
	lastCharHederIndex = 0;
	startTime = time(NULL);
	VserSimilars = _VserSimilars;
}

Request::~Request()
{
}


// ************** utils ***********

// encoding

void encodinString(std::string& str) // ! to do
{
	size_t index;
	std::string result;

	while ((index = str.find("%")) < str.size() - 2)
	{
		str = str.substr(0, index) + (str.substr(index + 1, 2)) + str.substr(index + 3);
	}
}

// ************ Getters **************

const LocationBlock& Request::getLocation() const
{
	return (location);
}

const std::string& Request::getURI() const
{
	return (this->URI);
}

const std::string& Request::getMethod() const
{
	return (Methods[MethodType - 1]);
}

const std::string& Request::getCgiPath(const std::string& extention) const
{
	const std::map<std::string, std::string>& cgi_ = location.getCgiPaths();
	std::map<std::string, std::string>::const_iterator it = cgi_.find(extention);
	return (it->second);
}

int Request::getMethdType() const
{
	return (this->MethodType);
}

int Request::getFdSocket() const
{
	return (SocketFd);
}

int Request::getTransferMode()
{
	return (TransferMode);
}

int Request::getError(void) const
{
	return this->ErrorFlag;
}

const std::string& Request::getBody(void) const
{
	return this->body;
}

const std::string& Request::getChunkedBodySize(void) const
{
	return this->chunkedBodySize;
}

const std::string& Request::getOldPath() const
{
	return (oldPath);
}

const std::string& Request::getNewPath() const
{
	return (newPath);
}

bool Request::getDoneServing() const
{
	return (this->doneServing);
}

bool Request::getDoneHeaderReading() const
{
	return (this->doneHeaderReading);
}

const std::map<std::string, std::string>& Request::getHeaders() const
{
	return (this->Header);
}

const std::string& Request::getQueryString() const
{
	return (this->QueryString);
}

int	Request::getByteReading() const
{
	return (bytesRead);
}

std::string Request::getHost(void) const
{
	std::string host;
	std::map<std::string, std::string>::const_iterator it;
	it = Header.find("host");
	if (it != Header.end())
	{
		host = it->second;
		host = split(host, ':')[0];
	}
	return host;
}

// ************* Setters *************

void Request::setPath(std::string str)
{
	this->newPath = str;
}

void Request::setDoneServing()
{
	this->doneServing = true;
}

void Request::setDoneHeaderReading()
{
	this->doneHeaderReading = true;
}

void Request::setLocation_str(std::string _location_str)
{
	this->location_str = _location_str;
}

void Request::setFlagError(int error_flag, const std::string& mes)
{
	ErrorFlag = error_flag;
	throw std::runtime_error(mes);
}

void Request::setFlagErrorWithoutThrow(int error_flag, const std::string& mes)
{
	(void)mes;
	ErrorFlag = error_flag;
}

/// @brief I check if there is /r at the last of any line, and ":" ater any key
/// and I map evry key with its value, afeter removing "\r"
/// @param line : the hole line from the heaader without "\n"

void Request::storeHeader(const std::string& line)
{
	std::string key;
	std::string value;
	size_t index;

	index = line.find(":");
	if (index == std::string::npos)
		setFlagError(BAD_REQ, "bad Request1");
	key = toLower(line.substr(0, index));
	value = skipLeadingWhitespace(line.substr(index + 1));
	Header.insert(std::make_pair(key, value));
}

void Request::storeData(const std::string& dataRequest)
{
	std::istringstream iss(dataRequest);
	std::string line;

	for (int i = 0; std::getline(iss, line); i++)
	{
		if (i == 0)
		{
			storeRequestLine(line);
		}
		else
		{
			storeHeader(line);
		}
	}
	if (Header.find("host") == Header.end())
		setFlagError(BAD_REQ, "bad Request_");
}

void Request::saveFirstChuckBody()
{
	for (int i = lastCharHederIndex; i < bytesRead; i++)
	{
		body += buf[i];
	}
}

void Request::storeBody()
{
	body.clear();
	if (lastCharHederIndex)
	{
		saveFirstChuckBody();
		lastCharHederIndex = 0;
		return;
	}
	body = std::string(buf, bytesRead);
}

// *************** Private Error Chicking Methods *******************

void Request::checkValidHeader()
{
}
bool Request::is_allowed_Method(const std::string& method) const
{
	const std::vector<std::string>& allowMethods = location.getAllowMethods();
	for (size_t i = 0; i < (allowMethods).size(); i++)
	{
		if (allowMethods[i].compare(method) == 0)
			return true;
	}
	return (false);
}

void Request::checkValid_GET_Header()
{
	if (is_allowed_Method("GET") == false)
		setFlagError(METHOD_NOT_ALLOWED, "Method not allwed");
}
// bool isPosNumber(std::string const& str)

// int stringToPosInt(std::string const& str)

void Request::checkValid_POST_Header()
{
	if (is_allowed_Method("POST") == false)
		setFlagError(METHOD_NOT_ALLOWED, "Method not allwed");
	if (Header.find("transfer-encoding") == Header.end() && Header.find("content-length") == Header.end())
	{
		setFlagError(LENGTH_REQUIRED, "Length Required");
	}
	if (this->Header.find("transfer-encoding") != Header.end() && Header["transfer-encoding"] != "chunked") // ! what is this
	{
		setFlagError(NOT_IMPLEMENTED, "Not Implemented");
	}
	if (Header.find("content-length") != Header.end())
	{
		if (isPosNumber(Header["content-length"]) == false)
		{
			setFlagError(BAD_REQUEST, "Bad request");
		}
		if (stringToPosInt(Header["content-length"]) < 0 || atoll((Header["content-length"]).c_str()) > Vserver.getMaxBodySize())
		{
			setFlagError(REQ_ENTITY_TOO_LONG, "Request Entity Too Large");
		}
	}
}

void Request::checkValid_DELETE_Header()
{
	if (is_allowed_Method("DELETE") == false)
		setFlagError(METHOD_NOT_ALLOWED, "Method not allwed");
}

void Request::checkValidMethod()
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
		setFlagError(BAD_REQ, "Bad Req");
		break;
	}
}

bool Request::URI_ValidLength(const std::string& uri) const
{
	return (uri.size() > MAX_URI_CHAR_SIZE);
}

bool Request::URI_ValidChar(const std::string& uri) const
{
	for (size_t i = 0; i < uri.size(); i++)
	{
		if (!std::isdigit(uri[i]) && !std::isalpha(uri[i]) && validChars.find(uri[i]) == std::string::npos)
		{
			return (1);
		}
	}
	return (0);
}

bool Request::URI_ValidLocation(const std::string& uri)
{
	const std::map<std::string, LocationBlock>& loc = Vserver.getLocations();

	mapIterType it_begin = loc.begin();
	mapIterType it_end = loc.end();
	for (; it_begin != it_end; ++it_begin)
	{
		if (uri.compare(0, (it_begin->first).size(), it_begin->first) == 0)
		{
			location_str = it_begin->first;
			if ((it_begin->first).size() == uri.size())
				return (0);
			if ((it_begin->first).size() < uri.size() && uri[(it_begin->first).size()] == '/')
				return (0);
			if (it_begin->first == "/")
				return (0);
		}
	}
	return (1);
}

void Request::URI_Checking(const std::string& uri)
{
	if (URI_ValidChar(uri))
	{
		setFlagError(BAD_REQ, "BAD_REQ");
	}
	if (URI_ValidLength(uri))
	{
		setFlagError(REQ_URI_TOO_LONG, "REQ_URI_TOO_LONG");
	}
	// if (URI_ValidLocation(uri))
	// {
	// 	setFlagError(NOT_FOUND, "NOT FOUND");
	// }
}

void Request::httpVersionCheck(const std::string& http)
{
	if (http.compare("HTTP/1.1"))
		setFlagError(BAD_REQ, "BAD REQUEST5");
}

void Request::WhichMethod(const std::string& method) // ! add other methods
{
	for (int i = 0; i < 8; i++)
	{
		if (Methods[i].compare(method) == 0)
		{
			if (i > 2)
			{
				MethodType = i + 1;
				setFlagError(NOT_IMPLEMENTED, "NOT IMPLEMENTED"); // TODO jkdkvcjhfdkvjdf
			}
			MethodType = i + 1;
			return;
		}
	}
	setFlagError(BAD_REQUEST, "bad Request.");
}

void Request::parseURI_QueryString(const std::string& client_uri)
{
	size_t index;

	if ((index = client_uri.find("?")) != std::string::npos)
	{
		this->URI = client_uri.substr(0, index);
		if (index != client_uri.size() - 1)
			this->QueryString = client_uri.substr(index + 1);
	}
	else
		this->URI = client_uri;
	encoding(this->URI);
}
/// @brief check for the uri if it has some tricky
/// @param path

void Request::checkRealPath(const std::string& path)
{
	std::string tmpPath = path;
	char		resolvedPath[MAX_URI_CHAR_SIZE + 2];
	size_t pos = tmpPath.rfind("/");

	if (pos != std::string::npos)
		tmpPath = tmpPath.substr(0, pos);

	if (realpath(tmpPath.c_str(), resolvedPath) == NULL)
	{
		return;
	}
	if (std::string(resolvedPath) != tmpPath)
	{
		setFlagError(FORBIDDEN, "forbidden-");
	}
}

void Request::storeRequestLine(const std::string& line)
{
	std::stringstream sstr(line);
	std::string word;

	for (int i = 0; sstr >> word; i++)
	{
		RequestLine.push_back(word);
	}
	if (RequestLine.size() != 3)
		setFlagError(BAD_REQ, "BAD REQUEST6");

	WhichMethod(RequestLine[0]);
	parseURI_QueryString(RequestLine[1]);
	URI_Checking(RequestLine[1]);
	httpVersionCheck(RequestLine[2]);
	oldPath = this->URI;
	// oldPath = getPathFromUri(URI, location.getLocationName());
}
//! this just a function not a method
std::string	getPathFromUri(const std::string& uri, const std::string& locationName)
{
	std::string	path;
	path = uri.substr(locationName.length());
	return (path);
}
//! oldpath is the uri
void Request::SetNewPath()
{
	
	newPath = location.getRoot();
	std::string	tmpPath = getPathFromUri(URI, location.getLocationName());
	if (!tmpPath.empty() && tmpPath[0] == '/')
		tmpPath.erase(0, 1);
	newPath += tmpPath;
	UriWithotLoc = "/" + tmpPath;
}

// ************** Main Methods *******************

bool	isSameSubstring(const std::string& uri, const std::string& locationName)
{
	for (size_t i = 0; i < locationName.length(); i++)
	{
		if (uri[i] != locationName[i])
		{
			return (false);
		}
	}
	if (uri.size() > locationName.size())
		return (uri[locationName.size()] == '/' || locationName == "/");
	return (uri.size() == locationName.size());
}

void Request::matchClients()
{
	const std::map<std::string, LocationBlock>& loc = Vserver.getLocations();

	mapIterType iter = loc.end();
	mapIterType it_begin = loc.begin();
	iter--;
	while (1)
	{
		if (isSameSubstring(URI, iter->first))
		{
			location = iter->second;
			return;
		}
		if (iter == it_begin)
		{
			setFlagError(BAD_GATEWAY, "no location match the request uri");
		}
		iter--;
	}
	location = iter->second;
}

void Request::timeOutCheching()
{
	if (doneHeaderReading)
		return;
	if (time(NULL) - startTime > TIMEOUT)
		setFlagErrorWithoutThrow(REQUEST_TIMEOUT, "request timeout");
}

void Request::checkReturn()
{
	if (!location.getReturnPath().empty())
		setFlagError(MOVED_PERMA, "Moved Permanently");
}

void	Request::checkIfUriExist(const std::string newPath)
{
	if (access(newPath.c_str(), F_OK))
	{ 
		setFlagError(NOT_FOUND, "not found1");
	}
}

bool Request::ReadCheckHeader()
{
	if (!doneHeaderReading)
	{
		for (int i = 0; i < bytesRead; i++)
		{
			// save the separated request
			req += buf[i];
			if (req.find("\r\n\r\n") != std::string::npos)
			{
				// store and parce the header
				storeData(HeaderReq);
				// save the start od the body if any
				lastCharHederIndex = i + 1;
				// set done reading header request , to not enter again
				doneHeaderReading = true;
				// match the clitent with its location
				assignServer();
				matchClients();

				// check if the method is valid, allowed 
				checkValidMethod();
				// afeter match the client, check if the locat
				// checkNewPath();
				checkReturn();
				// merge the uri with the root
				SetNewPath();
				// check for the uri
				checkRealPath(newPath);
				checkIfUriExist(newPath);
				return (true);
			}
			if (buf[i] != '\r')
			{
				// save the request header without \r and the last \n
				HeaderReq += buf[i];
			}
		}
	}
	return (false);
}

void Request::assignServer(void)
{
    if (VserSimilars.empty() || VserSimilars.find(Vserver.getSid()) == VserSimilars.end())
        return;
    if (getHost().empty() || Vserver.findHostName(getHost()))
        return;
    std::vector<ServerBlock>::iterator it = VserSimilars[Vserver.getSid()].begin();
    for (; it != VserSimilars[Vserver.getSid()].end(); it++)
    {
        if (it->findHostName(getHost()))
        {
            it->setFdSocket(Vserver.getFdSocket());
            Vserver = *it;
            Vserver = *it;
        }
    }
}

void Request::ReadRequest()
{
	try
	{
		bytesRead = read(SocketFd, buf, BUF_SIZE);

		if (bytesRead < 0)
		{
			setFlagError(READ_FAILED, "read system call failed");
		}
		if (bytesRead == 0)
		{
			// if (!doneHeaderReading)
				// setFlagError(READ_FAILED, "");// why u came here if there there is nothoing to read.
			return;
		}
		//* if ReadCheckHeader() return false , the reading request is done
		if (ReadCheckHeader() && !checked)
		{
			// checkValidMethod();
			// after replacing checkValidMethod() this checked has no meaning
			checked = true;
		}
		if (doneHeaderReading)
		{
			// * save the first chunck body
			storeBody();
		}
	}
	catch (const std::exception& e)
	{
	}
}

// ************ Debug *****************

// void Request::printRequest()
// {
// 	std::cout << "||************REQUEST HEADER************||\n";
// 	std::cout << "Method: " << Methods[MethodType - 1] << "\n";
// 	std::cout << "URI: " << URI << "\n";
// 	// std::cout << "URI: " << URI << "\n";

// 	for (std::map<std::string, std::string>::iterator it = Header.begin(); it != Header.end(); ++it)
// 	{
// 		std::cout << it->first << ":" << it->second << "\n";
// 	}
// 	std::cout << "query sting: " << getQueryString() << "\n";
// 	std::cout << "||************REQUEST Body************||\n";
// 	std::cout << body << "\n";
// 	std::cout << "||************************************||\n";
// }
