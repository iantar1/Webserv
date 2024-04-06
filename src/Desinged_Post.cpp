/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Desinged_Post.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 03:18:28 by nabboune          #+#    #+#             */
/*   Updated: 2024/04/06 08:11:55 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

void	Response::fillResponse(void)
{
	if (this->request->getDoneServing())
		return;

	if (this->request->getError()) {
		errorPage(this->request->getError());
		this->request->setDoneServing();
		return;
	}

	std::map<int, std::string>	mappedString = readFileIntoString(this->output_file);

	if (mappedString[0] == "Error") {
		std::cout << "1" << std::endl;
		thePostInternalServerError();
	}
	else {
		this->cgiBody = mappedString[1];
		std::map<std::string, std::string>::iterator	it = this->cgiResponseHeaders.find("STATUS");

		if (it != this->cgiResponseHeaders.end())
			this->response += "HTTP/1.1 " + it->second + "\r\n";
		else
			this->response += "HTTP/1.1 200 OK";
		it = this->cgiResponseHeaders.begin();
		while (it != this->cgiResponseHeaders.end()) {
			if (it->first == "STATUS") {
				it++;
				continue;
			}
			else if (it->first == "CONTENT-LENGHT")
				this->response += it->first + ": " + ToString(this->cgiBody.size()) + "\r\n";
			else {
				this->response += it->first + ": " + it->second + "\r\n";
			}
			it++;
		}
		it = this->cgiResponseHeaders.find("CONTENT-LENGHT");
		if (it == this->cgiResponseHeaders.end())
			this->response += "CONTENT-LENGHT: " + ToString(this->cgiBody.size()) + "\r\n";

		this->response += "\r\n" + this->cgiBody;
	}
	this->request->setDoneServing();
	// std::cout << "=========================\n" << this->response << "\n=========================" << std::endl;
}

void	Response::timedOut(void)
{
	time(&this->now);
	double	deltaT = difftime(this->now, this->start);
	std::cout << "DeltaT = " << deltaT << std::endl;
	if (deltaT > TIMEOUT) {
		errorPage(GATEWAY_TIMEOUT);
		this->request->setDoneServing();
		unlink(this->uploadedFileName.c_str());
	}
}

void	Response::posting(void)
{
	if (!prePostMethod())
		return ;

	if (this->request->getBody().empty()) {
		time(&this->now);
		double	deltaT = difftime(this->now, this->start);
		if (deltaT > TIMEOUT) {
			errorPage(GATEWAY_TIMEOUT);
			this->request->setDoneServing();
			unlink(this->uploadedFileName.c_str());
			return ;
		}
	}
	else
		PostResponse();
}

void	Response::PostResponse()
{
	if (this->request->location.getUploadEnable())
		thePostMethod();
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

	if (this->request->getHeaders().find("content-type") != this->request->getHeaders().end()) {
		this->contentType = this->request->getHeaders().find("content-type")->second;
		if (startsWith(this->contentType, "multipart/form-data")) {
			errorPage(NOT_IMPLEMENTED);
			this->request->setDoneServing();
			return false;
		}
	}

	if (!this->startedPostTime) {
		time(&this->start);
		this->startedPostTime = true;
	}

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
		this->gotTime = true;
	}

	if (!this->dataCopy)
	{
		if (!this->modeChecked)
		{
			if (this->request->getHeaders().find("transfer-encoding") != this->request->getHeaders().end())
				this->mode = CHUNKED;
			else
				this->mode = NORMAL;
			this->modeChecked = true;
		}
		
		if (this->mode == NORMAL)
		{
			this->postType = NORMAL_POST;
			if (!isNumeric(this->request->getHeaders().find("content-length")->second)) {
				errorPage(BAD_REQ);
				this->request->setDoneServing();
				return false;
			}
			char *end;
			this->contentLenght = std::strtoll((this->request->getHeaders().find("content-length")->second).c_str(), &end, 10);
			if (this->contentLenght < 0) {
				std::cout << "Hhhhhhh" << std::endl;
				errorPage(BAD_REQ);
				this->request->setDoneServing();
				return false;
			}
			else if (this->contentLenght > this->request->Vserver.getMaxBodySize()) {
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

		if (!this->outOpened) {
			this->outFile.open(this->uploadedFileName.c_str(), std::ios::app);
			this->outOpened = true;
		}
		if (this->contentLenght == 0) {
			this->outFile.close();
			errorPage(CREATED);
			this->request->setDoneServing();
			return false;
		}
	}
	return true;
}

void	Response::thePostMethod(void)
{
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
	this->request->setDoneServing();
}

void	Response::thePostResponseCreatedPage(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/201.htm") + "\r\n\r\n";
	thePostHeaderResponse(CREATED, CONTENT_LENGHT);
	this->response += this->body;
	this->request->setDoneServing();
}

void	Response::thePostResponseCreate(void)
{
	if (this->postType == NORMAL_POST)
	{
		std::cout << " || " << std::endl;
		this->outFile.write(this->request->getBody().data(), this->request->getBody().size());
		this->outFile << std::flush;
		time(&this->start);
		this->contentTotalSizePosted += this->request->getBody().size();
		// std::cout << "ZZZZ" << std::endl;
		// if (this->con)
		if (this->contentTotalSizePosted >= this->contentLenght){
			errorPage(GATEWAY_TIMEOUT);
			this->request->setDoneServing();
			unlink(this->uploadedFileName.c_str());
		}
		else if (this->contentTotalSizePosted == this->contentLenght) {
			this->outFile.close();
			
			// ! ila salite l posting hani declaritha lik....
			this->postingDone = true;

			// ! ila maghaddkhoulche asslane 3endek f CGI tanrespondi nichane...
			if (!isCGI()) {
				thePostResponseCreatedPage();
				this->request->setDoneServing();
				std::cout << "AAAA" << std::endl;
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
						this->request->setDoneServing();
						return;
					}

					// ! ila salite l posting hani declaritha lik....
					this->postingDone = true;

					if (!isCGI())
						thePostResponseCreatedPage();
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
			time(&this->start);
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
					this->request->setDoneServing();
					return;
				}

				// ! ila salite l posting hani declaritha lik....
				this->postingDone = true;

				if (!isCGI())
					thePostResponseCreatedPage();
				return;
			}
		}
	}
}