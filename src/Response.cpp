/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:09:09 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/28 07:48:47 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::Response(Request *request, t_files &files) : contentTotalSizePosted(0),
													   request(request), files(files), chunkStart(false), streamStart(false),
													   outOpened(false), gotTime(false), modeChecked(false), dataCopy(false),
													   startedTheChunk(false), doneCGI(false)
{
	std::cout << GREEN << "RESPONSE CONSTRUCTOR\n";
	this->socket = request->getFdSocket();
}

Response::~Response(void)
{

	std::cout << GREEN << "RESPONCE DESTRUCTOR\n"
			  << RESET;
	output_file.empty() && unlink(output_file.c_str());
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
	write(this->socket, this->response.c_str(), this->response.size());
	this->response.clear();
}

void Response::StartResponse()
{
	if (request->getError() != 0)
	{
		errorPage(request->getError());
		request->setDoneServing();
		return;
	}
	if (request->getMethdType() == GET)
	{
		if (isCGI() == true)
		{
			cgi_Handler();
		}
		theGetMethod();
	}
	else if (request->getMethdType() == POST)
	{
		PostResponse();
		if (isCGI() == true)
		{
			cgi_Handler();
			if (this->request->getError())
				errorPage(this->request->getError());
			else
				thePostResponseCreatedPage();
			this->request->setDoneServing();
		}
	}
	else if (request->getMethdType() == DELETE)
	{
		DeleteMethod();
		this->request->setDoneServing();
	}
}

const std::string &Response::getResponse() const
{
	return this->response;
}

// *************** Seters *********

void Response::setURI()
{
	this->uri = this->request->getURI();
}
