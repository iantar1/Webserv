/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:01:19 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/28 03:26:00 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

void	Response::PostResponse()
{
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

void	Response::thePostMethod()
{
	std::string										extension;
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
		this->uploadedFileName = "Uploads/" + generateNameFile(this->strTime2) + extension;

		this->response = "";
	}

	if (!this->outOpened) {
		this->outFile.open(this->uploadedFileName.c_str(), std::ios::app);
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
	if (this->postType == NORMAL_POST)
	{
		this->outFile.write(this->request->getBody().data(), this->request->getBody().size());
		this->outFile << std::flush;
		this->contentTotalSizePosted += this->request->getBody().size();
		if (this->contentTotalSizePosted == this->contentLenght) {
			this->outFile.close();
			if (!isCGI()) {
				this->request->setDoneServing();
				thePostResponseCreatedPage();
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
					if (!isCGI()) {
						this->request->setDoneServing();
						thePostResponseCreatedPage();
					}
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
			this->outFile << std::flush;
			this->appendedSize = this->appendedRequest.size();
			this->chunkStart = false;

			if (this->appendedRequest.find("0\r\n\r\n") != std::string::npos) {
				this->outFile.close();
				this->appendedRequest.clear();
				if (!isCGI()) {
					thePostResponseCreatedPage();
					this->request->setDoneServing();
				}
				return;
			}
		}
	}
}
