/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:23:35 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/23 05:32:04 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

Request::RequestException::RequestException(std::string error) : error(error) {}

const char*	Request::RequestException::what(void) const throw() { return this->error.c_str(); }

Request::RequestException::~RequestException(void) throw() {}

Request::Request(void) {}

Request::Request(std::string strRequest)
{
	int												i = 0;
	std::string										key, value, method, path, protocol, chunkedContentLenght;
	std::map<std::string, std::string>::iterator	itl, ite;

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
	this->method.insert(std::make_pair("Method", method));
	this->method.insert(std::make_pair("Path", path));
	this->method.insert(std::make_pair("Protocol", protocol));
	// this->body = "";
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

	while (strRequest[i] && (strRequest[i] == '\r' || strRequest[i] == '\n'))
		i++;

	this->body = strRequest.substr(i);

	// else if (ite != this->request.end())
	// {
	// 	while (request[i] && request[i] != '\r'
	// 		&& request[i + 1] && request[i + 1] != '\n')
	// 	{
	// 		chunkedContentLenght += request[i];
	// 		std::string	str(1, request[i++]);
	// 		this->body.append(str);
	// 	}
	// 	i++;
	// 	this->body.append("\r\n");
	// 	ccl = hexStringToInt(chunkedContentLenght);
	// 	while (j < ccl)
	// 	{
	// 		std::string	str(1, request[i + j++]);
	// 		this->body.append(str);
	// 	}
	// }
	// std::cout << "******" << itl->second << std::endl;
	// else if (key != "" && value == "")
	// {
	// key.resize(key.size() - 1);
	// if (request[i])
	// 	while(request[i])
	// 		key += request[i++];
	// this->body += key;
	// }
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

std::map<std::string, std::string>	Request::getRequest(void) const { return this->request; }

std::map<std::string, std::string>	Request::getMethod(void) const { return this->method; }

void	Request::checkMethod(void) const
{
	std::map<std::string, std::string>::iterator	it;
	std::map<std::string, std::string> copy = this->method;

	it = easyfind(copy, "Method");
	if (it->second != "GET" && it->second != "POST" && it->second != "DELETE" && it->second != "")
		throw (RequestException(it->second + ": Method Not Supported Right Now !"));
	it = easyfind(copy, "Protocol");
	if (it->second != "HTTP/1.1")
		throw (RequestException(it->second + ": Protocol Not Supported Right Now!"));
}

Request::~Request(void) {}


std::map<std::string, std::string>::iterator	Request::easyfind(std::map<std::string, std::string> &container, std::string x) const
{
	std::map<std::string, std::string>::iterator	it = container.begin();
	while (it != container.end())
	{
		if (it->first == x)
			return it;
		it++;
	}
	return it;
}
