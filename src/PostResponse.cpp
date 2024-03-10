/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 04:13:45 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/10 15:53:23 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/PostResponse.hpp"
# include "../includes/utils.hpp"

PostResponse::PostResponseException::PostResponseException(std::string error) : error(error) {}

const char *PostResponse::PostResponseException::what(void) const throw() { return this->error.c_str(); }

PostResponse::PostResponseException::~PostResponseException(void) throw() {}


PostResponse::PostResponse(int socket, Request *request, t_files files)
{
	this->request = request;
	this->socket = socket;
	this->files = files;
	this->contentTotalSizePosted = 0;

	std::cout << YELLOW << this->request->getBody() << RESET << std::endl;
	this->request->printRequest();
	std::cout << RED << "ACHE HAD IKHAN : " << this->request->getRequest().find("Content-Length")->first << RESET << std::endl;
	if (this->request->getRequest().find("Content-Length") != this->request->getRequest().end())
		this->mode = NORMAL;
	else
		this->mode = CHUNKED;
	thePostMethod(&this->mode);
}


PostResponse::~PostResponse(void) {}

void	PostResponse::thePostMethod(int *mode)
{
	tm												*local_time;
	time_t											now;
	std::string										extension, fileName;
	std::map<std::string, std::string>				mime, req;
	std::map<std::string, std::string>::iterator	mime_it, req_it;

	now = time(0);

	req = this->request->getRequest();
	req_it = req.find("Content-Length");
	std::cout << "SALAM O3ALAYKOUME!!!!!!!!!\n";
	std::cout << *mode << std::endl;
	if (*mode == NORMAL)
	{
		this->postType = NORMAL_POST;
		this->contentLenght = std::atoi((this->request->getRequest().find("Content-Length")->second).c_str());
	}
	else
		this->postType = CHUNKED_POST;

	this->contentType = this->request->getRequest().find("Content-Type")->second;
	extension = getContentExtension(this->files.mime, this->contentType);
	fileName = "Uploads/" + generateNameFile() + extension;

	std::cout << RED << "FILE NAME : " << fileName << RESET << std::endl;

	local_time = localtime(&now);
	this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	this->response = "";

	this->outFile.open(fileName.c_str(), std::ios::binary);

	if (!this->outFile.is_open())
		thePostInternalServerError();
	else
		thePostResponseCreate(mode);

	this->outFile.close();
}

void		PostResponse::thePostHeaderResponse(int code, int transferType)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(code)->second;

	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += this->contentType;

	if (transferType == CONTENT_LENGHT)
	{
		header_it = this->files.headers.find(CONTENT_LENGHT);
		header_it->second += ToString(this->body.size());
	}

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if ((transferType == TRANSFER_ENCODING && header_it->first != CONTENT_LENGHT)
			|| (transferType == CONTENT_LENGHT && header_it->first != TRANSFER_ENCODING))
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\r\n";
}

void	PostResponse::thePostInternalServerError(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/500.htm") + "\r\n\r\n";
	thePostHeaderResponse(INTERNAL_ERR, CONTENT_LENGHT);
	this->response += this->body;
	write(this->socket, this->response.c_str(), this->response.size());
	std::cout << "WRITE\n";
}

void	PostResponse::thePostResponseCreatedPage(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/201.htm") + "\r\n\r\n";
	thePostHeaderResponse(CREATED, CONTENT_LENGHT);
	this->response += this->body;
	write(this->socket, this->response.c_str(), this->response.size());
	std::cout << "WRITE\n";
}

void	PostResponse::thePostResponseCreate(int *mode)
{
	int	ccl;
	std::cout << "========||==========\n";
	if (this->postType == NORMAL_POST)
	{
		std::string		data = this->request->getBody();
		this->outFile.write(data.data(), data.size());
		this->contentTotalSizePosted += data.size();
		std::cout << RED << this->contentTotalSizePosted << RESET << std::endl;
		if (this->contentTotalSizePosted == this->contentLenght) {
			this->request->setDoneServing();
			thePostResponseCreatedPage();
			std::cout << "||TT||TT||TT||TT||TT||TT||\n";
		}
	}
	else if (this->postType == CHUNKED_POST)
	{
		std::string		data = this->request->getBody();
		ccl = hexStringToInt(this->request->getChunkedBodySize());
		std::cout << ccl << " || " << this->request->getChunkedBodySize() << std::endl;
		if (ccl != 0)
			this->outFile.write(data.data(), ccl);
			// write(this->socket, this->request.getBody().data(), ccl);
		else
			*mode = NORMAL;
	}
}
