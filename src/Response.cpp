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

Response::Response(int socket, Request &request, t_files files, int error, int *mode)
{
	this->socket = socket;
	this->request = request;
	this->files = files;

	// std::string		data = this->request.getBody();
	// std::cout << "==> " << data << std::endl;

	tm		*local_time;
	time_t	now;

	local_time = localtime(&now);
	this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);

	if (error != 0)
	{
		errorPage(error);
		return;
	}

	std::map<std::string, std::string> method = this->request.getMethod();

	if (method.find("Method")->second == "GET")
		GetResponse		get(this->socket, this->request, this->files);
	else if (method.find("Method")->second == "POST")
		PostResponse	post(this->socket, this->request, this->files, mode);
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

void	Response::errorPage(int errorCode)
{
	std::map<int, std::string>::iterator	header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(errorCode)->second;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += "text/html";

	header_it = this->files.headers.find(CONTENT_LENGHT);
	this->responseBody = getPageContent("defaultPages/" + ToString(errorCode) + ".htm") + "\r\n\r\n";
	header_it->second += ToString(responseBody.size());

	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if (header_it->first != TRANSFER_ENCODING)
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\r\n" + this->responseBody;
	write(this->socket, this->response.c_str(), this->response.size());
}
