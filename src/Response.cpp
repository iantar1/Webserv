/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 01:05:52 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/18 03:07:29 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::ResponseException::ResponseException(std::string error) : error(error) {}

const char *Response::ResponseException::what(void) const throw() { return this->error.c_str(); }

Response::ResponseException::~ResponseException(void) throw() {}

// Response::Response(void) {}

Response::Response(int socket, Request &request, std::map<std::string, std::string> mime)
{
	this->socket = socket;
	this->request = request;
	this->mime = mime;
	this->header.insert(std::make_pair("0", "HTTP/1.1 "));
	this->header.insert(std::make_pair("1", "Content-Type: "));
	this->header.insert(std::make_pair("2", "Content-Length: "));
	this->header.insert(std::make_pair("3", "Date: "));
	this->header.insert(std::make_pair("4", "Server: Muguiwarra"));
	this->header.insert(std::make_pair("5", "Transfer-Encoding: "));

	std::map<std::string, std::string> copy = this->request.getMethod();

	if (easyfind(copy, "Method")->second == "GET")
		theGetMethod();
	std::cout << " !!!! Done !!!! " << std::endl;
}

Response::Response(const Response &other) : header(other.header), body(other.body) {}

Response &Response::operator=(const Response &other)
{
	this->body = other.body;
	this->header = other.header;
	return (*this);
}

Response::~Response(void) {}

void Response::theGetMethod(void)
{
	time_t now = time(0);
	std::map<std::string, std::string>::iterator header_it, mime_it;
	std::map<std::string, std::string> copy = this->request.getMethod();
	std::map<std::string, std::string>::iterator it = easyfind(copy, "Path");
	const char *path = (it->second).c_str();
	std::string extension = getFileExtension(it->second);
	std::string contentType;

	if (extension == "")
		contentType = "application/octet-stream";
	else
	{
		mime_it = easyfind(this->mime, extension);
		contentType = mime_it->second;
	}

	std::string line;
	std::ifstream inFile;
	inFile.open(path + 1);

	struct stat fileStat;
	if (stat(path + 1, &fileStat) != 0)
		throw(ResponseException("Error getting file status"));

	if (S_ISDIR(fileStat.st_mode))
		std::cout << path << " is a directory, and its size is : " << fileStat.st_size << std::endl;
	else if (S_ISREG(fileStat.st_mode))
		std::cout << path << " is a regular file, and its size is : " << fileStat.st_size << std::endl;
	else
		std::cout << path << " is neither a directory nor a regular file, and its size is : " << fileStat.st_size << std::endl;

	tm *local_time = localtime(&now);
	std::string strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	this->response = "";
	std::cout << "10" << std::endl;
	if (!inFile.is_open())
	{
		header_it = easyfind(this->header, "0");
		header_it->second += "404 Not Found";
		header_it = easyfind(this->header, "1");
		header_it->second += "text/plain";
		header_it = easyfind(this->header, "2");
		this->body = "Error 404!\r\n\n";
		header_it->second += ToString(body.size());
		header_it = easyfind(this->header, "3");
		header_it->second += strTime;

		header_it = this->header.begin();
		while (header_it != this->header.end())
		{
			if (header_it->first != "5")
				this->response += header_it->second + "\r\n";
			header_it++;
		}
		this->response += "\n" + this->body;
		write(this->socket, this->response.c_str(), this->response.size());
	}
	else
	{
		std::cout << "9" << std::endl;
		header_it = easyfind(this->header, "0");
		header_it->second += "200 OK";
		header_it = easyfind(this->header, "1");
		header_it->second += contentType;
		header_it = easyfind(this->header, "5");
		// header_it->second += ToString(body.size());
		header_it->second += "chunked";
		header_it = easyfind(this->header, "3");
		header_it->second += strTime;
		std::cout << "8" << std::endl;

		header_it = this->header.begin();
		while (header_it != this->header.end())
		{
			if (header_it->first != "2")
				this->response += header_it->second + "\r\n";
			header_it++;
		}
		this->response += "\r\n";
		std::cout << "7" << std::endl;
		write(this->socket, this->response.c_str(), this->response.size());

		// std::cout << "HEADER : \n" << header << "\n====" <<std::endl;

		this->body = "";
		std::cout << "6" << std::endl;
		char buf[1025];
		std::streamsize byteRead = 1;
		size_t len = 0;

		// Dunno Why The Video Isn't Sent By The Webserv !!

		while (byteRead > 0)
		{
			inFile.read(buf, 1024);
			byteRead = inFile.gcount();
			if (byteRead <= 0)
				break;
			std::string str(buf, byteRead);

			this->body = dec_to_hex(str.size()) + "\r\n" + str + "\r\n";
			int i = write(this->socket, this->body.c_str(), this->body.length());
			len += str.size();
			if (i < 0)
			{
				if (errno == ECONNRESET)
					continue;
				perror("write : ");
			}
		}
		std::cout << "len : " << len << std::endl;
		std::cout << "5 " << std::endl;
	}
	std::cout << "3" << std::endl;
	write(this->socket, "0\r\n\r\n", 5);
	inFile.close();

	std::cout << "4" << std::endl;
}

std::map<std::string, std::string>::iterator Response::easyfind(std::map<std::string, std::string> &container, std::string x) const
{
	std::map<std::string, std::string>::iterator it = container.begin();
	while (it != container.end())
	{
		if (it->first == x)
			return it;
		it++;
	}
	return it;
}
