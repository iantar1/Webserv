/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 01:05:52 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/06 10:29:56 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/macros.hpp"
# include "../includes/GetResponse.hpp"
# include "../includes/PostResponse.hpp"
# include "../includes/Response.hpp"
# include "../includes/utils.hpp"

Response::ResponseException::ResponseException(std::string error) : error(error) {}

const char *Response::ResponseException::what(void) const throw() { return this->error.c_str(); }

Response::ResponseException::~ResponseException(void) throw() {}

Response::Response(Request* request, t_files* files)
{

	this->request = request;
	this->socket = request->getFdSocket();
	this->files = files;

	// std::string		data = this->request->getBody();
	// std::cout << "==> " << data << std::endl;

	// tm		*local_time;
	// time_t	now;

	// local_time = localtime(&now);
	// this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);

	// * if (request->getError() != 0)
	// * {
	// * 	errorPage(request->getError());
	// * 	return ;
	// * }
	// std::cout << "getMethosd : " << request->getMethdType() << "\n";
	// if (request->getMethdType() == GET)
	// {
	// 	std::cout << GREEN << " GET " << RESET << "\n"; 
	// 	GetResponse		get(this->socket, this->request, this->files);
	// }
	// else if (request->getMethdType() == POST)
	// 	PostResponse	post(this->socket, this->request, this->files);
    //else Delete
}


Response::~Response(void) {}

void	Response::errorPage(int errorCode)
{
	std::map<int, std::string>::iterator	header_it;

	header_it = this->files->headers.find(RESPONSE_STATUS);
	header_it->second += this->files->status.find(errorCode)->second;

	header_it = this->files->headers.find(CONTENT_TYPE);
	header_it->second += "text/html";

	header_it = this->files->headers.find(CONTENT_LENGHT);
	this->responseBody = getPageContent("defaultPages/" + ToString(errorCode) + ".htm") + "\r\n\r\n";
	header_it->second += ToString(responseBody.size());

	header_it = this->files->headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files->headers.begin();
	while (header_it != this->files->headers.end())
	{
		if (header_it->first != TRANSFER_ENCODING)
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\r\n" + this->responseBody;
	write(this->socket, this->response.c_str(), this->response.size());
}

void	Response::StartResponse()
{
	std::cout << "Satrt Response\n";
	if (request->getMethdType() == GET)
	{
		std::cout << GREEN << " GET " << RESET << "\n"; 
		GetResponse		get(this->socket, this->request, this->files);
	}
	// else if (request->getMethdType() == POST)
	// 	PostResponse	post(this->socket, this->request, this->files);
    //else Delete
}