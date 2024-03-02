/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:23:35 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/28 23:36:34 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

Request::RequestException::RequestException(std::string error) : error(error) {}

const char*	Request::RequestException::what(void) const throw() { return this->error.c_str(); }

Request::RequestException::~RequestException(void) throw() {}

Request::Request(void) {}

Request::Request(std::string strRequest, int *mode)
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
