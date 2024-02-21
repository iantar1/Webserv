/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 03:21:39 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/21 04:09:40 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/GetResponse.hpp"
#include "../includes/utils.hpp"

GetResponse::GetResponseException::GetResponseException(std::string error) : error(error) {}

const char *GetResponse::GetResponseException::what(void) const throw() { return this->error.c_str(); }

GetResponse::GetResponseException::~GetResponseException(void) throw() {}


GetResponse::GetResponse(int socket, Request &request, t_files files)
{
	this->request = request;
	this->socket = socket;
	this->files = files;

	theGetMethod();
}

GetResponse::GetResponse(const GetResponse &other) : request(other.request), files(other.files), socket(other.socket) {}

GetResponse &GetResponse::operator=(const GetResponse &other)
{
	this->request = other.request;
	this->socket = other.socket;
	this->files = other.files;
	return (*this);
}

GetResponse::~GetResponse(void) {}

void GetResponse::theGetErrorNotFound(void)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(NOT_FOUND)->second;
	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += "text/plain";
	header_it = this->files.headers.find(CONTENT_LENGHT);
	this->body = "Error 404!\r\n\n";
	header_it->second += ToString(body.size());
	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if (header_it->first != TRANSFER_ENCODING)
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\n" + this->body;
	write(this->socket, this->response.c_str(), this->response.size());
}

void GetResponse::theGetResponseOk(void)
{
	std::map<int, std::string>::iterator header_it;

	struct stat fileStat;
	if (stat(this->path.c_str() + 1, &fileStat) != 0)
		throw(GetResponseException((this->path + "Error getting file stats")));

	// if (S_ISDIR(fileStat.st_mode))
	// 	std::cout << path << " is a directory, and its size is : " << fileStat.st_size << std::endl;
	// else if (S_ISREG(fileStat.st_mode))
	// 	std::cout << path << " is a regular file, and its size is : " << fileStat.st_size << std::endl;
	// else
	// 	std::cout << path << " is neither a directory nor a regular file, and its size is : " << fileStat.st_size << std::endl;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(OK)->second;
	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += contentType;
	header_it = this->files.headers.find(DATE);
	header_it->second += strTime;

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if (header_it->first != CONTENT_LENGHT)
			this->response += header_it->second + "\r\n";
		header_it++;
	}

	this->response += "\r\n";
	write(this->socket, this->response.c_str(), this->response.size());

	this->body = "";
	char buf[1025];
	std::streamsize byteRead = 1;

	while (byteRead > 0)
	{
		signal(SIGPIPE, SIG_IGN);
		inFile.read(buf, 1024);
		byteRead = inFile.gcount();
		if (byteRead <= 0)
			break;
		std::string str(buf, byteRead);

		this->body = dec_to_hex(str.size()) + "\r\n" + str + "\r\n";
		write(this->socket, this->body.c_str(), this->body.length());
	}

	write(this->socket, "0\r\n\r\n", 5);
}

void GetResponse::theGetMethod(void)
{
	tm												*local_time;
	time_t											now;
	std::string										extension, line;
	std::map<int, std::string>::iterator 			header_it;
	std::map<std::string, std::string>::iterator	mime_it;

	now = time(0);
	this->path = this->request.getMethod().find("Path")->second;
	extension = getFileExtension(this->path);
	if (extension != "")
	{
		mime_it = this->files.mime.find(extension);
		if (mime_it != this->files.mime.end())
			this->contentType = mime_it->second;
	}
	else
		this->contentType = "applocation/octet-stream";

	local_time = localtime(&now);
	this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	this->response = "";

	this->inFile.open(this->path.c_str() + 1);

	if (!inFile.is_open())
		theGetErrorNotFound();
	else
		theGetResponseOk();

	inFile.close();
}
