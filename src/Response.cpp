/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 01:05:52 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 04:10:49 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/Webserv.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::ResponseException::ResponseException(std::string &error) : error(error) {}

const char*	Response::ResponseException::what(void) const throw() { return this->error.c_str(); }

Response::ResponseException::~ResponseException(void) throw() {}

// Response::Response(void) {}

Response::Response(int socket, Request &request, std::map<std::string, std::string> mime)
{
	this->socket = socket;
	this->request = request;
	this->mime = mime;
	this->header.insert(std::make_pair("0", "HTTP/1.1 "));
	this->header.insert(std::make_pair("1", "Content-Type: "));
	// this->header.insert(std::make_pair("2", "Content-Length: "));
	this->header.insert(std::make_pair("2", "Transfer-Encoding: "));
	this->header.insert(std::make_pair("3", "Date: "));
	this->header.insert(std::make_pair("4", "Server: Muguiwarra"));

	std::map<std::string, std::string> copy = this->request.getMethod();

	if (easyfind(copy, "Method")->second == "GET")
		theGetMethod();
}

Response::Response(const Response &other) : header(other.header), body(other.body) {}

Response&	Response::operator=(const Response &other)
{
	this->body = other.body;
	this->header = other.header;
	return (*this);
}

Response::~Response(void) {}

void	Response::theGetMethod(void)
{
	time_t	now = time(0);
	std::map<std::string, std::string>::iterator	header_it, mime_it;
	std::map<std::string, std::string> copy = this->request.getMethod();
	std::map<std::string, std::string>::iterator	it = easyfind(copy, "Path");
	const char *path = (it->second).c_str();
	std::string	line;
	std::ifstream	inFile(path + 1);
	mime_it = easyfind(this->mime, it->second);

/*
	===> Check if the path is a File or a Directory ! <===

	struct stat fileStat;
	if (stat(path, &fileStat) != 0) {
		std::cerr << "Error getting file status\n";
		return 1;
	}

	if (S_ISDIR(fileStat.st_mode)) {
		std::cout << path << " is a directory\n";
	} else if (S_ISREG(fileStat.st_mode)) {
		std::cout << path << " is a regular file\n";
	} else {
		std::cout << path << " is neither a directory nor a regular file\n";
	}
*/
	
	// std::cout << mime_it->first << " || " << mime_it->second << std::endl;
	// std::cout << it->second << std::endl;
	std::string		contentType = mime_it->second;
	// std::cout << contentType << std::endl;
	tm	*local_time = localtime(&now);
	std::string	strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	this->response = "";
	if (!inFile.is_open())
	{
		header_it = easyfind(this->header, "0");
		header_it->second += "404 Not Found";
		header_it = easyfind(this->header, "1");
		header_it->second += "text/plain";
		header_it = easyfind(this->header, "2");
		// header_it->second += ToString(body.size());
		header_it->second += "chunked";
		header_it = easyfind(this->header, "3");
		header_it->second += strTime;

		header_it = this->header.begin();
		while (header_it != this->header.end())
		{
			this->response += header_it->second + "\r\n";
			header_it++;
		}
		write(this->socket, this->response.c_str(), this->response.size());

		line = "Error 404!";
		this->body = dec_to_hex(line.size()) + "\r\n" + line + "\r\n";
		write(this->socket, this->body.c_str(), this->body.size());
	}
	else
	{
		header_it = easyfind(this->header, "0");
		header_it->second += "200 OK";
		header_it = easyfind(this->header, "1");
		header_it->second += contentType;
		header_it = easyfind(this->header, "2");
		// header_it->second += ToString(body.size());
		header_it->second += "chunked";
		header_it = easyfind(this->header, "3");
		header_it->second += strTime;

		header_it = this->header.begin();
		while (header_it != this->header.end())
		{
			this->response += header_it->second + "\r\n";
			header_it++;
		}
		write(this->socket, this->response.c_str(), this->response.size());

		// std::cout << "HEADER : \n" << header << "\n====" <<std::endl;

		this->body = "";
		while (std::getline(inFile, line))
		{
			line += "\n";
			this->body = dec_to_hex(line.size()) + "\r\n" + line + "\r\n";				
			write(this->socket, this->body.c_str(), this->body.size());
		}
	}
	write(this->socket, "0\r\n\r\n", 5);
}
