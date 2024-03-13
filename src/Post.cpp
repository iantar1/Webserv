/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:01:19 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/13 20:15:57 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

void	Response::PostResponse()
{
	if (!this->modeChecked)
	{
		if (this->request->getRequest().find("content-length") != this->request->getRequest().end())
			this->mode = NORMAL;
		else {
			std::cout << "========||==========\n";
			this->mode = CHUNKED;
		}
		this->modeChecked = true;
	}
	thePostMethod();
}

void	Response::thePostMethod()
{
	std::string										extension, fileName;
	std::map<std::string, std::string>				mime;
	std::map<std::string, std::string>::iterator	mime_it;

	if (!this->gotTime)
	{
		tm												*local_time;
		time_t											now;

		now = time(0);
		local_time = localtime(&now);
		this->strTime = ToString(local_time->tm_year + 1900) + "-"
			+ ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday)
				+ " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min)
					+ ":" + ToString(local_time->tm_sec);
	}

	if (!this->dataCopy)
	{
		if (this->mode == NORMAL)
		{
			this->postType = NORMAL_POST;
			this->contentLenght = std::atoi((this->request->getRequest().find("content-length")->second).c_str());
		}
		else
			this->postType = CHUNKED_POST;
		this->dataCopy = true;
		this->contentType = this->request->getRequest().find("content-type")->second;

		extension = getContentExtension(this->files.mime, this->contentType);
		fileName = "Uploads/" + generateNameFile() + extension;

		this->response = "";
	}

	if (!this->outOpened) {
		this->outFile.open(fileName.c_str(), std::ios::app);
		this->outOpened = true;
	}

	if (!this->outFile.is_open())
		thePostInternalServerError();
	else
		thePostResponseCreate();
}

void		Response::thePostHeaderResponse(int code, int transferType)
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

void	Response::thePostInternalServerError(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/500.htm") + "\r\n\r\n";
	thePostHeaderResponse(INTERNAL_ERR, CONTENT_LENGHT);
	this->response += this->body;
}

void	Response::thePostResponseCreatedPage(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/201.htm") + "\r\n\r\n";
	thePostHeaderResponse(CREATED, CONTENT_LENGHT);
	this->response += this->body;
}

void	Response::thePostResponseCreate(void)
{
	// std::cout << "========||==========\n";
	// std::cout << this->request->getBody() << "\n";
	// std::cout << "========||==========\n";
	// this->request->printRequest();
	if (this->postType == NORMAL_POST)
	{
		this->outFile.write(this->request->getBody().data(), this->request->getBody().size());
		// std::cout << YELLOW << "=================||=================" << RESET << std::endl;
		// std::cout << RED << "SIZE: " << this->request->getBody().size() << RESET << "\nDATA:\n" << this->request->getBody().data() << std::endl;
		// std::cout << YELLOW << "=================||=================" << RESET << std::endl;
		this->contentTotalSizePosted += this->request->getBody().size();
		// std::cout << YELLOW << "=================_-_-_=================" << RESET << std::endl;
		// std::cout << BLUE << this->contentTotalSizePosted << " || " << this->contentLenght << RESET << std::endl;
		// std::cout << YELLOW << "=================_-_-_=================" << RESET << std::endl;
		// std::cout << RED << this->contentLenght << RESET << std::endl;
		// std::cout << this->contentTotalSizePosted << "\n";
		if (this->contentTotalSizePosted == this->contentLenght) {
			this->request->setDoneServing();
			thePostResponseCreatedPage();
			this->outFile.close();
			// std::cout << "||TT||TT||TT||TT||TT||TT||\n";
		}
	}
	else if (this->postType == CHUNKED_POST)
	{
		size_t	offset = 0;

		while (offset < this->request->getBody().size()) {
			
			if (!this->streamStart) {
				size_t	eol = std::string(this->request->getBody().c_str() + offset).find("\r\n");
				this->chunkContentTotalSizePosted = 0;
				if (eol != std::string::npos) {
					// size_t	chunkSizeLineLength = eol - offset;
					this->chunkSize = hexStringToInt(std::string(this->request->getBody().c_str() + offset, eol + 1)); // eol => chunkSizeLineLength

					offset = eol + 2;
					if (this->chunkSize == 0) {
						this->outFile.close();
						break;
					}

					int		len = std::string(this->request->getBody().c_str() + offset).size();
					if (len <= this->chunkSize) {
						this->outFile.write(this->request->getBody().c_str() + offset, len - 2);
						this->contentTotalSizePosted += len - 2;
						this->chunkContentTotalSizePosted += len - 2;
						this->streamStart = true;
						offset += len + 2;
					}
					else {
						this->outFile.write(this->request->getBody().c_str() + offset, this->chunkSize);
						this->contentTotalSizePosted += this->chunkSize;
						offset += this->chunkSize + 2;
					}
				}
			} // Ache nahya had tkhwira li kayna f chunkSize !!!!!
			else {
				size_t	eol = std::string(this->request->getBody().c_str() + offset).find("\r\n");
				if (eol == std::string::npos)
					eol = std::string(this->request->getBody().c_str() + offset).size() - 1;
				this->outFile.write(this->request->getBody().c_str() + offset, eol + 1);
				this->contentTotalSizePosted += eol;
				this->chunkContentTotalSizePosted += eol;
				if (this->chunkContentTotalSizePosted == this->chunkSize) {
					this->chunkSize = 0;
					this->streamStart = false;
				}
				offset += this->chunkSize + 2;	

				// if (eol != std::string::npos) {
				// 		this->outFile.write(this->request->getBody().c_str() + offset, eol);
				// 		this->contentTotalSizePosted += eol;
				// 		this->chunkContentTotalSizePosted += eol;
				// 		if (this->chunkContentTotalSizePosted == this->chunkSize) {
				// 			this->chunkSize = 0;
				// 			this->streamStart = false;
				// 		}
				// 		offset += this->chunkSize + 2;
				// }
				// else {
				// 		this->outFile.write(this->request->getBody().c_str() + offset, std::string(this->request->getBody().c_str() + offset).size());
				// 		this->contentTotalSizePosted += std::string(this->request->getBody().c_str() + offset).size();
				// 		this->chunkContentTotalSizePosted += std::string(this->request->getBody().c_str() + offset).size();
				// 		if (this->chunkContentTotalSizePosted == this->chunkSize) {
				// 			this->chunkSize = 0;
				// 			this->streamStart = false;
				// 		}
				// 		offset += this->chunkSize + 2;
				// }
				
			}
		}
	}
}
