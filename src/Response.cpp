/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:09:09 by nabboune          #+#    #+#             */
/*   Updated: 2024/06/02 19:07:02 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::Response(Request *request, t_files &files) : contentTotalSizePosted(0),
													   request(request), files(files), chunkStart(false), streamStart(false),
													   outOpened(false), gotTime(false), modeChecked(false), dataCopy(false),
													   startedTheChunk(false), preCGI(false), responseDone(0), doneCGI(false)
{
	this->socket = request->getFdSocket();
	childToKill = false;
	indexExist = false;
	oneTime = 0;
}

Response::~Response()
{
	unlink(output_cgi.c_str());
}

void Response::errorPage(int errorCode)
{
	std::map<int, std::string>::iterator header_it;

	this->response.clear();

	if (request->getMethod() != "HEAD")
		this->body = getPageContent("defaultPages/" + ToString(errorCode) + ".htm") + "\r\n\r\n";

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(errorCode)->second;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += "text/html";

	header_it = this->files.headers.find(CONTENT_LENGHT);
	this->responseBody = getPageContent("defaultPages/" + ToString(errorCode) + ".htm") + "\r\n\r\n";
	header_it->second += ToString(responseBody.size());

	tm* local_time;
	time_t now;

	now = time(0);
	local_time = localtime(&now);

	this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	if (!this->cookie.empty())
	{
		header_it = this->files.headers.find(SET_COOKIE);
		header_it->second += this->cookie;
	}
	else
		this->files.headers.erase(SET_COOKIE);

	if (errorCode == MOVED_PERMA && !this->request->getLocation().getReturnPath().empty())
		this->redirection = this->request->getLocation().getReturnPath();

	if (!this->redirection.empty())
	{
		header_it = this->files.headers.find(LOCATION);
		header_it->second += this->redirection;
	}
	else
		this->files.headers.erase(LOCATION);

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if (header_it->first != TRANSFER_ENCODING)
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\r\n";

	if (request->getMethod() != "HEAD")
		this->response += this->body;
}

void Response::servPage(std::string page)
{
	std::map<int, std::string>::iterator header_it;


	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(OK)->second;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += "text/html";

	header_it = this->files.headers.find(CONTENT_LENGHT);
	this->responseBody = getPageContent(page) + "\r\n\r\n";
	header_it->second += ToString(responseBody.size());

	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if (header_it->first != TRANSFER_ENCODING)
			this->response += header_it->second + "\r\n";
		header_it++;
	}
	this->response += "\r\n" + this->responseBody;
}

void Response::cgiResponse(void)
{
	if (this->request->getMethdType() == POST)
		this->cgiResponseHeaders["STATUS"] = "201 CREATED";
	else if (this->request->getMethdType() == GET)
		this->cgiResponseHeaders["STATUS"] = "200 OK";
	else if (this->request->getMethdType() == DELETE)
		this->cgiResponseHeaders["STATUS"] = "204 NO CONTENT";
	this->cgiResponseHeaders["CONTENT-TYPE"] = "text/html";
	this->cgiResponseHeaders["CONTENT-LENGTH"] = "";
	this->cgiResponseHeaders["Server"] = "Tekshbila";
	this->constructedCgiHeader = true;
}

void Response::StartResponse()
{

	if (isCGI() && !this->constructedCgiHeader)
		cgiResponse();
	if (request->getError() != 0)
	{
		errorPage(request->getError());
		return;
	}

	if (request->getMethdType() == GET)
	{
		if (!checkPreGetMethod())
			return;

		if (isCGI() == true)
		{
			cgi_Handler();
			if (this->doneCGI)
				fillResponse();
		}
		else
			theGetMethod();
	}
	else if (request->getMethdType() == POST)
	{
		if (!this->postingDone)
			posting();
		if (isCGI() && this->postingDone)
		{
			cgi_Handler();
			if (this->doneCGI)
				fillResponse();
		}

	}
	else if (request->getMethdType() == DELETE)
	{
		DeleteMethod();
	}
}

const std::string &Response::getResponse() const
{
	return this->response;
}

std::string Response::getPageContent(std::string page)
{
	std::string pgNbStr = (split(page, '/').back());
	std::istringstream pg(pgNbStr);
	struct stat data;
	int pgNb;

	pg >> pgNb;
	if (request->Vserver.getErrorPages().find(pgNb) != request->Vserver.getErrorPages().end()
		&& (stat(request->Vserver.getErrorPages().find(pgNb)->second.c_str(), &data) != -1)
		&& (S_ISREG(data.st_mode)))
	{
		page = request->Vserver.getErrorPages().find(pgNb)->second;
	}
	std::ifstream inFile(page.c_str());
	std::string line, pageContent = "";

	
	if (inFile.is_open())
	{
		while (std::getline(inFile, line))
		{
			pageContent += line;
			pageContent += "\n";
		}
	}
	inFile.close();
	return pageContent;
}
// *************** Seters *********

void Response::setURI()
{
	this->uri = this->request->getURI();
}
