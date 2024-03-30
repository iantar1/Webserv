/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:01:19 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/30 10:25:07 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

void	Response::PostResponse()
{
	if (this->request->location.getUploadEnable()) {
		if (!this->modeChecked)
		{
			if (this->request->getHeaders().find("transfer-encoding") != this->request->getHeaders().end())
				this->mode = CHUNKED;
			else
				this->mode = NORMAL;
			this->modeChecked = true;
		}
		thePostMethod();
	}
	else {
		if (isCGI())
			cgi_Handler();
		theGetMethod();
	}
}

bool	Response::prePostMethod(void)
{
	std::string										extension;
	std::string										uploadPath;
	std::map<std::string, std::string>				mime;
	std::map<std::string, std::string>::iterator	mime_it;

	if (!this->gotTime)
	{
		tm												*local_time;
		time_t											now;

		now = time(0);
		srand(now);
		local_time = localtime(&now);
		this->strTime = ToString(local_time->tm_year + 1900) + "-"
			+ ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday)
				+ " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min)
					+ ":" + ToString(local_time->tm_sec);
		this->strTime2 = ToString(local_time->tm_year + 1900) + "-"
			+ ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday)
				+ "_" + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min)
					+ ":" + ToString(local_time->tm_sec);
	}

	if (!this->dataCopy)
	{
		if (this->mode == NORMAL)
		{
			this->postType = NORMAL_POST;
			this->contentLenght = std::atoi((this->request->getHeaders().find("content-length")->second).c_str());
			if (this->contentLenght > this->request->Vserver.getMaxBodySize()) {
				errorPage(LARGE_REQ);
				this->request->setDoneServing();
				return false;
			}
		}
		else
			this->postType = CHUNKED_POST;
		this->dataCopy = true;

		if (this->request->getHeaders().find("content-type") != this->request->getHeaders().end()) {
			this->contentType = this->request->getHeaders().find("content-type")->second;
			extension = getContentExtension(this->files.mime, this->contentType);
		}
		else
			extension = "";

		if (!isDirectory(this->request->location.getUploadPath().c_str()) && !isCGI()) {
			theGetErrorForbidden();
			this->request->setDoneServing();
			return false;
		}
		else
		{
			uploadPath = this->request->location.getUploadPath();
			if (!endsWith(this->request->location.getUploadPath(), "/"))
				uploadPath += "/";
			this->uploadedFileName = uploadPath + generateNameFile(this->strTime2) + extension;
			this->response = "";
		}
	}
	return true;
}

void	Response::thePostMethod(void)
{
	if (!prePostMethod())
		return;

	if (!this->outOpened) {
		this->outFile.open(this->uploadedFileName.c_str(), std::ios::app);
		this->outOpened = true;
	}
	if (!this->outFile.is_open() && !isCGI())
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
	if (this->postType == NORMAL_POST)
	{
		this->outFile.write(this->request->getBody().data(), this->request->getBody().size());
		this->outFile << std::flush;
		this->contentTotalSizePosted += this->request->getBody().size();
		// std::cout << "ZZZZ" << std::endl;
		if (this->contentTotalSizePosted == this->contentLenght) {
			this->outFile.close();
			if (!isCGI()) {
				this->request->setDoneServing();
				thePostResponseCreatedPage();
				// std::cout << "AAAA" << std::endl;
			}
			else if (isCGI()) {
				/*
					====================================================
					Fine tate7ete l output d CGI ? bach n9ede nservihe !
					====================================================
				*/
				// std::cout << "UUUU" << std::endl;
				cgi_Handler();
				if (this->request->getError())
					errorPage(this->request->getError());
				// else
				// 	thePostResponseCreatedPage(); // Hna khassni nservi dakchi li tayretourni CGI
			}
		}
	}
	else if (this->postType == CHUNKED_POST)
	{
		this->appendedRequest.append(this->request->getBody());
		if (this->chunkStart == false) {
			size_t	eol = this->appendedRequest.find("\r\n");
			if (eol != std::string::npos) {
				this->chunkSize = hexStringToInt(this->appendedRequest);

				if (this->chunkSize == 0) {
					this->outFile.close();
					this->appendedRequest.clear();
					if (this->contentTotalSizePosted > this->request->Vserver.getMaxBodySize()) {
						errorPage(LARGE_REQ);
						unlink(this->uploadedFileName.c_str());
					}
					else if (!isCGI())
						thePostResponseCreatedPage();
					else if (isCGI()) {
						/*
							====================================================
							Fine tate7ete l output d CGI ? bach n9ede nservihe !
							====================================================
						*/
						cgi_Handler();
						if (this->request->getError())
							errorPage(this->request->getError());
						// else
						// 	thePostResponseCreatedPage(); // Hna khassni nservi dakchi li tayretourni CGI
					}
					this->request->setDoneServing();
					return;
				}

				this->chunkStart = true;
				this->appendedRequest = this->appendedRequest.substr(eol + 2);
				this->appendedSize = this->appendedRequest.size();
			}
			else
				return;
		}
		else
			this->appendedSize = this->appendedRequest.size();
		if (this->appendedSize >= this->chunkSize + 2) {
			std::string	toWrite = this->appendedRequest.substr(0, this->chunkSize);
			this->appendedRequest = this->appendedRequest.substr(this->chunkSize + 2);
			this->outFile.write(toWrite.data(), this->chunkSize);
			this->contentTotalSizePosted += toWrite.size();
			this->outFile << std::flush;
			this->appendedSize = this->appendedRequest.size();
			this->chunkStart = false;

			if (this->appendedRequest.find("0\r\n\r\n") != std::string::npos) {
				this->outFile.close();
				this->appendedRequest.clear();
				if (this->contentTotalSizePosted > this->request->Vserver.getMaxBodySize()) {
					errorPage(LARGE_REQ);
					unlink(this->uploadedFileName.c_str());
				}
				else if (!isCGI())
					thePostResponseCreatedPage();
				else if (isCGI()) {
					/*
						====================================================
						Fine tate7ete l output d CGI ? bach n9ede nservihe !
						====================================================
					*/
					cgi_Handler();
					if (this->request->getError())
						errorPage(this->request->getError());
					// else
					// 	thePostResponseCreatedPage(); // Hna khassni nservi dakchi li tayretourni CGI
				}
				this->request->setDoneServing();
				return;
			}
		}
	}
}
