/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 01:05:52 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/23 04:26:42 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/GetResponse.hpp"
#include "../includes/PostResponse.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::ResponseException::ResponseException(std::string error) : error(error) {}

const char *Response::ResponseException::what(void) const throw() { return this->error.c_str(); }

Response::ResponseException::~ResponseException(void) throw() {}

Response::Response(int socket, Request &request, t_files files)
{
	this->socket = socket;
	this->request = request;
	this->files = files;

	// std::string		data = this->request.getBody();
	// std::cout << "==> " << data << std::endl;

	std::map<std::string, std::string> method = this->request.getMethod();

	if (method.find("Method")->second == "GET")
		GetResponse		get(this->socket, this->request, this->files);
	else if (method.find("Method")->second == "POST")
		PostResponse	post(this->socket, this->request, this->files);
}

Response::Response(const Response &other) : socket(other.socket), files(other.files), request(other.request) {}

Response &Response::operator=(const Response &other)
{
	this->request = other.request;
	this->socket = other.socket;
	this->files = other.files;
	return (*this);
}

Response::~Response(void) {}
