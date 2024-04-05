/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:09:09 by nabboune          #+#    #+#             */
/*   Updated: 2024/04/03 06:02:55 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::Response(Request *request, t_files &files) : contentTotalSizePosted(0),
													   request(request), files(files), chunkStart(false), streamStart(false),
													   outOpened(false), gotTime(false), modeChecked(false), dataCopy(false),
													   startedTheChunk(false), doneCGI(false), preCGI(false)
{
	std::cout << GREEN << "RESPONSE CONSTRUCTOR\n";
	this->socket = request->getFdSocket();
}

Response::~Response(void)
{

	std::cout << GREEN << "RESPONCE DESTRUCTOR\n"
			  << RESET;
	// output_file.empty() && unlink(output_file.c_str());
}

void Response::errorPage(int errorCode)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(errorCode)->second;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += "text/html";

	header_it = this->files.headers.find(CONTENT_LENGHT);
	this->responseBody = getPageContent("defaultPages/" + ToString(errorCode) + ".htm") + "\r\n\r\n";
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
	// write(this->socket, this->response.c_str(), this->response.size());
	// this->response.clear();
}

void Response::servPage(std::string page)
{
	std::map<int, std::string>::iterator header_it;

	// std::cout << "===> " << this->request->location.getIndex().at(0) << std::endl;

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

void	Response::cgiResponse(void)
{
	std::cout << YELLOW << "###" << RESET << std::endl;
	if (this->request->getMethdType() == POST)
		this->cgiResponseHeaders["STATUS"] = "201 CREATED";
	else if (this->request->getMethdType() == GET)
		this->cgiResponseHeaders["STATUS"] = "200 OK";
	else if (this->request->getMethdType() == DELETE)
		this->cgiResponseHeaders["STATUS"] = "204 NO CONTENT";
	this->cgiResponseHeaders["CONTENT-TYPE"] = "text/html";
	this->cgiResponseHeaders["CONTENT-LENGHT"] = "";
	this->cgiResponseHeaders["SERVER"] = "Tekshbila";
}

void Response::StartResponse()
{
	if (isCGI())
		cgiResponse();
	std::cout << RED << "XXX" << RESET << std::endl;
	if (request->getError() != 0)
	{
		errorPage(request->getError());
		request->setDoneServing();
		return;
	}
	if (request->getMethdType() == GET)
	{// ! you need to check is the file exist or not
		// std::cout << "Response : " << this->response << std::endl;
		if (isCGI() == true) {
			cgi_Handler();
			if (this->doneCGI)
				fillResponse();
		}
		else
			theGetMethod();
	}
	else if (request->getMethdType() == POST)
	{
		// PostResponse();
		if (!this->postingDone)
			posting();
		if (isCGI() && this->postingDone)
		{
			cgi_Handler();
			std::cout << "doneCGI : " << this->doneCGI << std::endl;
			if (this->doneCGI)
				fillResponse();
		}
	}
	else if (request->getMethdType() == DELETE)
	{
		DeleteMethod();
		this->request->setDoneServing();
	}
	std::cout << RED << "YYY" << RESET << std::endl;
}

const std::string &Response::getResponse() const
{
	return this->response;
}

std::string	Response::getPageContent(std::string page)
{
	std::string		pgNbStr = (split(page, '/').back());
	std::istringstream pg(pgNbStr);
	int pgNb;
	pg >> pgNb;
	if (request->Vserver.getErrorPages().find(pgNb) != request->Vserver.getErrorPages().end())
	{
		page =  request->Vserver.getErrorPages().find(pgNb)->second;
	}
	std::ifstream	inFile(page.c_str());
	std::string		line, pageContent = "";
 
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
