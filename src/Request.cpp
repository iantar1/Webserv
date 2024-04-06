/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:03:11 by iantar            #+#    #+#             */
/*   Updated: 2024/04/06 06:01:16 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

std::string Request::Methods[] = {"POST", "GET", "DELETE", "HEAD", "PUT", "CONNECT", "OPTIONS", "TRACE"};
std::string Request::validChars = "-._~:/?#[]@!$&'()*+,;=%";

Request::Request(int fd, const ServerBlock &_Vserver) : SocketFd(fd), ErrorFlag(0), 
				doneServing(false), doneHeaderReading(false), Vserver(_Vserver)
{
	std::cout << YELLOW << "REQUEST CONSTRUCTOR\n";
	MethodType = 0;
	lastCharHederIndex = 0;
	startTime = time(NULL);
}

Request::~Request()
{
}

// ************** utils ***********

// encoding

std::string hexToSting(const std::string &hex)
{
	(void)hex;
	return (hex);
}

void encodinString(std::string &str) // ! to do
{
	size_t index;
	std::string result;

	while ((index = str.find("%")) < str.size() - 2)
	{
		str = str.substr(0, index) + hexToSting(str.substr(index + 1, 2)) + str.substr(index + 3);
	}
}

// ************ Getters **************

const LocationBlock &Request::getLocation() const
{
	return (location);
}

const std::string &Request::getURI() const
{
	return (this->URI);
}

const std::string &Request::getMethod() const
{
	return (Methods[MethodType - 1]);
}

const std::string &Request::getCgiPath(const std::string &extention) const
{
	const std::map<std::string, std::string> &cgi_ = location.getCgiPaths();
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

const std::string &Request::getBody(void) const
{
	return this->body;
}

const std::string &Request::getChunkedBodySize(void) const
{
	return this->chunkedBodySize;
}

const std::string &Request::getOldPath() const
{
	return (oldPath);
}

const std::string &Request::getNewPath() const
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

const std::map<std::string, std::string> &Request::getHeaders() const
{
	return (this->Header);
}

const std::string &Request::getQueryString() const
{
	return (this->QueryString);
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

void Request::setFlagError(int error_flag, const std::string &mes)
{
	ErrorFlag = error_flag;
	throw std::runtime_error(mes);
}

void Request::setFlagErrorWithoutThrow(int error_flag, const std::string &mes)
{
	ErrorFlag = error_flag;
	std::cerr << mes << std::endl;
}

// std::string skipLeadingWhitespace(const std::string &str)
// {
// 	std::string result;
// 	size_t index;

// 	for (index = 0; index < str.length() && isspace(str[index]); ++index)
// 	{
// 	}
// 	return (str.substr(index));
// }

/// @brief I check if there is /r at the last of any line, and ":" ater any key
/// and I map evry key with its value, afeter removing "\r"
/// @param line : the hole line from the heaader without "\n"

void Request::storeHeader(const std::string &line)
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

void Request::storeData(const std::string &dataRequest)
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
bool Request::is_allowed_Method(const std::string &method) const
{
	const std::vector<std::string> &allowMethods = location.getAllowMethods();
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
		if (atoi((Header["content-length"]).c_str()) > Vserver.getMaxBodySize())
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

bool Request::URI_ValidLength(const std::string &uri) const
{
	return (uri.size() > MAX_URI_CHAR_SIZE);
}

bool Request::URI_ValidChar(const std::string &uri) const
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

bool Request::URI_ValidLocation(const std::string &uri)
{
	// mapIterType it_end = Vserver.getLocationsEndIterMap();

	const std::map<std::string, LocationBlock> &loc = Vserver.getLocations();

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
			if (it_begin->first == "/") // ! hard code
				return (0);
		}
	}
	return (1);
}

void Request::URI_Checking(const std::string &uri)
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

void Request::httpVersionCheck(const std::string &http)
{
	if (http.compare("HTTP/1.1"))
		setFlagError(BAD_REQ, "BAD REQUEST5");
}

void Request::WhichMethod(const std::string &method) // ! add other methods
{
	for (int i = 0; i < 8; i++)
	{
		if (Methods[i].compare(method) == 0)
		{
			if (i > 2)
				setFlagError(NOT_IMPLEMENTED, "NOT IMPLEMENTED");
			MethodType = i + 1;
			return;
		}
	}
	setFlagError(BAD_REQUEST, "bad Request.");
}

void Request::parseURI_QueryString(const std::string &client_uri)
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

void Request::storeRequestLine(const std::string &line)
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
	oldPath = this->URI; // URI
	SetNewPath();		 // set new Path
}

void Request::SetNewPath() // ! match location and change this
{
	std::cout << "old: " << oldPath << "\n";
	// ! use location.getRootLOcation() instead
	// Guade had had location...
	newPath = Vserver.getRoot();
	if (newPath[newPath.size() - 1] == '/')
		newPath.resize(newPath.size() - 1);
	newPath += oldPath; // * u need to handle when there is / at last of the root
	std::cout << "new: " << newPath << "\n";
}

// ************** Main Methods *******************

void Request::matchClients()
{
	const std::map<std::string, LocationBlock> &loc = Vserver.getLocations();

	mapIterType iter = loc.end();
	mapIterType it_begin = loc.begin();

	iter--;
	while (1)
	{
		if (iter->first.compare(0, iter->first.size(), URI.c_str(), iter->first.size()) == 0)
		{
			location = iter->second;
			return;
		}
		if (iter == it_begin)
		{
			setFlagError(NOT_FOUND, "no location match the request uri");
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

bool Request::ReadCheckHeader()
{
	if (!doneHeaderReading)
	{
		for (int i = 0; i < bytesRead; i++)
		{
			// save the separated request
			req += buf[i];
			if (req.size() > 3 && req.substr(req.size() - 4) == "\r\n\r\n")
			{
				// store and parce the header
				storeData(HeaderReq);
				// save the start od the body if any
				lastCharHederIndex = i + 1;
				// set done reading header request , to not enter again
				doneHeaderReading = true;
				// match the clitent with its location
				matchClients();
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

void Request::ReadRequest()
{
	try
	{
		bytesRead = read(SocketFd, buf, BUF_SIZE);
		if (bytesRead < 0)
			setFlagError(INTERNAL_ERR, "read system call failed");
		if (bytesRead == 0 && !doneHeaderReading)
		{
			setFlagError(BAD_REQ, "bad request7");
		}
		//* if ReadCheckHeader() return false , the reading request is done
		if (ReadCheckHeader())
		{
			checkValidMethod();
		}
		if (doneHeaderReading)
		{
			// * save the first chunck body
			if (MethodType == POST)
			{
				storeBody();
			}
		}
		// printRequest();
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << '\n';
	}
}

// ************ Debug *****************

void Request::printRequest()
{
	std::cout << "||************REQUEST HEADER************||\n";
	std::cout << "Method: " << Methods[MethodType - 1] << "\n";
	std::cout << "URI: " << URI << "\n";
	// std::cout << "URI: " << URI << "\n";

	for (std::map<std::string, std::string>::iterator it = Header.begin(); it != Header.end(); ++it)
	{
		std::cout << it->first << ":" << it->second << "\n";
	}
	std::cout << "query sting: " << getQueryString() << "\n";
	std::cout << "||************REQUEST Body************||\n";
	std::cout << body << "\n";
	std::cout << "||************************************||\n";
}
