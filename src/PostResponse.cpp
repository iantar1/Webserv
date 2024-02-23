/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 04:13:45 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/23 07:32:52 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/PostResponse.hpp"
#include "../includes/utils.hpp"

PostResponse::PostResponseException::PostResponseException(std::string error) : error(error) {}

const char *PostResponse::PostResponseException::what(void) const throw() { return this->error.c_str(); }

PostResponse::PostResponseException::~PostResponseException(void) throw() {}


PostResponse::PostResponse(int socket, Request &request, t_files files)
{
	this->request = request;
	this->socket = socket;
	this->files = files;

	thePostMethod();
}

PostResponse::PostResponse(const PostResponse &other) : request(other.request), files(other.files), socket(other.socket) {}

PostResponse &PostResponse::operator=(const PostResponse &other)
{
	this->request = other.request;
	this->socket = other.socket;
	this->files = other.files;
	return (*this);
}

PostResponse::~PostResponse(void) {}

void	PostResponse::thePostMethod(void)
{
	tm												*local_time;
	time_t											now;
	std::string										extension, fileName;
	std::map<std::string, std::string>				mime, req;
	std::map<std::string, std::string>::iterator	mime_it, req_it;

	now = time(0);
	
	req = this->request.getRequest();
	req_it = req.find("Content-Length");
	if (req_it != req.end())
		this->postType = NORMAL_POST;
	else
		this->postType = CHUNKED_POST;

	this->contentLenght = std::atoi((this->request.getRequest().find("Content-Length")->second).c_str());
	this->contentType = this->request.getRequest().find("Content-Type")->second;
	extension = getContentExtension(this->files.mime, this->contentType);
	fileName = "Uploads/" + generateNameFile() + extension;

	local_time = localtime(&now);
	this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	this->response = "";

	this->outFile.open(fileName.c_str(), std::ios::binary);

	if (!this->outFile.is_open())
		thePostInternalServerError();
	else
		thePostResponseCreated();

	this->outFile.close();
}

void	PostResponse::thePostInternalServerError(void)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(INTERNAL_ERR)->second;
	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += "text/plain";
	header_it = this->files.headers.find(CONTENT_LENGHT);
	this->resonseBody = getPageContent("defaultPages/500.htm") + "\r\n\n";
	header_it->second += ToString(resonseBody.size());
	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if (header_it->first != TRANSFER_ENCODING)
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\n" + this->resonseBody;
	write(this->socket, this->response.c_str(), this->response.size());
}

//	Dunno Why I Can't Post Properly The Binary Files...

void	PostResponse::thePostResponseCreated(void)
{
	// int	contentLenght;

	if (this->postType == NORMAL_POST)
	{
		std::string		data = this->request.getBody();
		// std::cout << "==> " << data << " <==" << std::endl;
		std::cout << "$*$ " << data << " $*$" << std::endl;
		// contentLenght = std::atoi(this->request.getRequest().find("Content-Length")->second.c_str());
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~> " << contentLenght << std::endl;
		this->outFile.write(data.data(), contentLenght);
		this->outFile << data;
	}
	// else if (this->postType == CHUNKED_POST)
	// {
		// while (request[i] != '\r\n')
		// 	chunkedContentLenght += request[i++];
		// i++;
		// ccl = hexStringToInt(chunkedContentLenght);
		// while (j < ccl)
		// {
		// 	std::string	str(1, request[i + j++]);
		// 	this->body.append(str);
		// }
	// }
}
