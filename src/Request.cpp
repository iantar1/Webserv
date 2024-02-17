/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:23:35 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 04:08:15 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"
#include "../includes/utils.hpp"

Request::RequestException::RequestException(std::string error) : error(error) {}

const char*	Request::RequestException::what(void) const throw() { return this->error.c_str(); }

Request::RequestException::~RequestException(void) throw() {}

Request::Request(void) {}

Request::Request(char *request)
{
	int									i = 0;
	std::string							key, value, method, path, protocol;

	protocol = "";
	method = "";
	path = "";
	while(request[i] && request[i] != ' ' && request[i] != '\n')
		method += request[i++];
	i++;
	while(request[i] && request[i] != ' ' && request[i] != '\n')
		path += request[i++];
	i++;
	while(request[i] && request[i] != ' ' && request[i] != '\n')
		protocol += request[i++];
	i++;
	this->method.insert(std::make_pair("Method", method));
	this->method.insert(std::make_pair("Path", path));
	this->method.insert(std::make_pair("Protocol", protocol));
	while(request[i])
	{
		value = "";
		key = "";
		while(request[i] && request[i] != '\n')
		{
			if(request[i] != ':')
			{
				key += request[i++];
				continue;
			}
			else
			{
				i += 2;
				while(request[i] && request[i] != '\n')
					value += request[i++];
			}
		}
		i++;
		if (key != "" && value != "")
			this->request.insert(std::make_pair(key, value));
	}
}

Request::Request(const Request &other) : request(other.request), method(other.method) {}

Request&	Request::operator=(const Request &other)
{
	this->request = other.request;
	this->method = other.method;
	return (*this);
}

std::map<std::string, std::string>	Request::getRequest(void) const
{
	return this->request;
}

std::map<std::string, std::string>	Request::getMethod(void) const
{
	return this->method;
}

void	Request::checkMethod(void) const
{
	std::map<std::string, std::string>::iterator	it;
	std::map<std::string, std::string> copy = this->method;

	it = easyfind(copy, "Protocol");
	std::cout << "2" << std::endl;
	std::cout << it->first << std::endl;
	if (it->second != "HTTP/1.1")
		throw (RequestException("Protocol Not Supported Right Now!"));
	it = easyfind(copy, "Method");
	if (it->second != "GET" && it->second != "POST" && it->second != "DELETE")
		throw (RequestException("Method Not Supported Right Now !"));
	std::cout << "3" << std::endl;
}

Request::~Request(void) {}
