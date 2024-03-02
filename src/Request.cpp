/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:23:35 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/02 07:52:22 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

Request::RequestException::RequestException(std::string error) : error(error) {}

const char*	Request::RequestException::what(void) const throw() { return this->error.c_str(); }

Request::RequestException::~RequestException(void) throw() {}

Request::Request(void) {}

Request::Request(std::string strRequest, int *mode, Location local) : local(local)
{
	int												i = 0;
	std::string										key, value, method, path, protocol, chunkedContentLenght;
	std::map<std::string, std::string>::iterator	itl, ite;

	this->errorCode = 0;
	this->body = "";
	if (*mode == NORMAL)
	{
		protocol = "";
		method = "";
		path = "";
		while(strRequest[i] && strRequest[i] != ' ')
			method += strRequest[i++];
		i++;
		while(strRequest[i] && strRequest[i] != ' ')
			path += strRequest[i++];
		i++;
		while(strRequest[i] && strRequest[i] != ' ' && strRequest[i] != '\r' && strRequest[i + 1] && strRequest[i + 1] != '\n')
			protocol += strRequest[i++];
		i += 2;
		
		if (!isAllowed(path))
		{
			this->errorCode = BAD_REQUEST;
			return ;
		}
		if (isLongReq(path))
		{
			this->errorCode = LONG_URI_REQ;
			return ;
		}

		if (std::find(this->local.getAllowedMethods().begin(), this->local.getAllowedMethods().end(), method) != this->local.getAllowedMethods().end())
		{
			this->errorCode = METHOD_NOT_ALLOWED;
			return ;
		}

		std::map<int, std::map<std::string, std::string> > redirectMap = this->local.getRedirect();
		std::map<int, std::map<std::string, std::string> >::iterator redirectIter = redirectMap.find(errorCode);
		if (redirectIter != redirectMap.end())
		{
			std::map<std::string, std::string> innerMap = redirectIter->second;
			std::map<std::string, std::string>::iterator innerIter = innerMap.find(path);
			if (innerIter != innerMap.end())
			{
				this->errorCode = this->local.getRedirect().begin()->first;
				path = innerIter->second;
			}
		}

		if (startsWith(path, this->local.getLocation()))
			path = replacePath(path, this->local.getLocation());
		else
		{
			this->errorCode = NOT_FOUND;
			return ;
		}


		// Check What Happens with the Path in the request !!!

		this->method.insert(std::make_pair("Method", method));
		this->method.insert(std::make_pair("Path", path));
		this->method.insert(std::make_pair("Protocol", protocol));
		if (!checkMethod())
			return ;
		while (strRequest[i] && strRequest[i] != '\r'
			&& strRequest[i + 1] && strRequest[i + 1] != '\n')
		{
			key.clear();
			value.clear();

			while (strRequest[i] && strRequest[i] != ':')
				key += strRequest[i++];
			i++;
			while (strRequest[i] && strRequest[i] == ' ')
				i++;
			while (strRequest[i] && strRequest[i] != '\r'
				&& strRequest[i + 1] && strRequest[i + 1] != '\n')
				value += strRequest[i++];
			i += 2;

			this->request.insert(std::make_pair(key, value));
		}

		itl = this->request.find("Content-Length");
		ite = this->request.find("Transfer-Encoding");
		if (method == "POST" && (itl == this->request.end() && ite == this->request.end()))
		{
			this->errorCode = BAD_REQUEST;
			return ;
		}
		if (itl != this->request.end() && ite != this->request.end())
		{
			this->errorCode = BAD_REQUEST;
			return ;
		}
		else if (ite != this->request.end() && ite->second != "chunked")
		{
			this->errorCode = NOT_IMPLEMENTED;
			return ;
		}

		while (strRequest[i] && (strRequest[i] == '\r' || strRequest[i] == '\n'))
			i++;

		if (ite != this->request.end())
		{
			*mode = CHUNKED;
			while (strRequest[i] && strRequest[i] != '\r'
				&& strRequest[i + 1] && strRequest[i + 1] != '\n')
			{
				std::cout << strRequest[i];
				this->chunkedBodySize += strRequest[i++];
			}

			while (strRequest[i] && (strRequest[i] == '\r' || strRequest[i] == '\n'))
				i++;
		}

		this->body = strRequest.substr(i);
		if (this->body.size() > this->local.getMaxBodySize())
			this->errorCode = LARGE_REQ;
	}
	else
	{
		i = 0;

		while (strRequest[i] && strRequest[i] != '\r'
			&& strRequest[i + 1] && strRequest[i + 1] != '\n')
			this->chunkedBodySize += strRequest[i++];

		while (strRequest[i] && (strRequest[i] == '\r' || strRequest[i] == '\n'))
			i++;
		std::cout << "********" << std::endl;
		this->body = strRequest.substr(i);
		std::cout << "****()****" << std::endl;
	}
}

Request::Request(const Request &other) : request(other.request), method(other.method), body(other.body) {}

Request&	Request::operator=(const Request &other)
{
	this->request = other.request;
	this->method = other.method;
	this->body = other.body;
	return (*this);
}

std::string							Request::getBody(void) const { return this->body; }

std::string							Request::getChunkedBodySize(void) const { return this->chunkedBodySize; }

int									Request::getError(void) const {	return this->errorCode; }

std::map<std::string, std::string>	Request::getRequest(void) const { return this->request; }

std::map<std::string, std::string>	Request::getMethod(void) const { return this->method; }

bool	Request::checkMethod(void)
{
	std::map<std::string, std::string>::iterator	it;
	std::map<std::string, std::string> copy = this->method;

	it = copy.find("Method");
	if (it->second != "GET" && it->second != "POST" && it->second != "DELETE" && it->second != "")
	{
		this->errorCode = METHOD_NOT_ALLOWED;
		return false;
	}
	it = copy.find("Protocol");
	if (it->second != "HTTP/1.1")
	{
		this->errorCode = METHOD_NOT_ALLOWED;
		return false;
	}
	return true;
}

Request::~Request(void) {}
