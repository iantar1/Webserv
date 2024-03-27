/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:56:53 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/27 07:32:34 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

// you should tell me where can I send to had zmmer Content-type: text/html; charset=UTF-8   from CGI

void Response::theGetHeaderResponse(int code, int transferType)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(code)->second;

	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += this->contentType;

	//! if (set_cookies())
	header_it = this->files.headers.find(SET_COOKIE);
	header_it->second += cookie;

	if (this->redirection != "")
	{
		header_it = this->files.headers.find(LOCATION);
		header_it->second += this->redirection;
	}

	if (transferType == CONTENT_LENGHT)
	{
		header_it = this->files.headers.find(CONTENT_LENGHT);
		header_it->second += ToString(this->body.size());
	}

	override your map here
	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if ((transferType == TRANSFER_ENCODING && header_it->first != CONTENT_LENGHT) || (transferType == CONTENT_LENGHT && header_it->first != TRANSFER_ENCODING))
		{
			this->response += header_it->second + "\r\n";
		}
		header_it++;
	}
	this->response += "\r\n";
}

void Response::theGetRedirectionRequest(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/301.htm") + "\r\n\r\n";
	this->redirection = this->oldPath + "/";
	theGetHeaderResponse(MOVED_PERMA, CONTENT_LENGHT);
	this->response += this->body;
}

void Response::theGetErrorBadRequest(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/400.htm") + "\r\n\r\n";
	theGetHeaderResponse(BAD_REQUEST, CONTENT_LENGHT);
	this->response += this->body;
}

void Response::theGetErrorNotFound(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/404.htm") + "\r\n\r\n";
	theGetHeaderResponse(NOT_FOUND, CONTENT_LENGHT);
	this->response += this->body;
	this->request->setDoneServing();
}

void Response::theGetErrorForbidden(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/403.htm") + "\r\n\r\n";
	theGetHeaderResponse(FORBIDDEN, CONTENT_LENGHT);
	this->response += this->body;
}

void Response::theGetResponseOk(void)
{
	if (!this->streamStart)
	{
		theGetHeaderResponse(OK, TRANSFER_ENCODING);
		this->streamStart = true;
	}
	else
	{
		this->body.clear();
		char buf[1025];
		std::streamsize byteRead;

		signal(SIGPIPE, SIG_IGN);
		this->inFile.read(buf, 1024);
		byteRead = this->inFile.gcount();
		if (byteRead <= 0)
		{
			this->body = "0\r\n\r\n";
			this->request->setDoneServing();
		}
		else
		{
			std::string str(buf, byteRead);
			this->body = dec_to_hex(str.size()) + "\r\n" + str + "\r\n";
		}
		this->response = this->body;
	}
}

void Response::directoryListing(void)
{
	DIR *dir;
	struct dirent *entry;
	std::ostringstream listing;

	listing << "<html><head><title>Index of " << this->oldPath << "</title></head><body>";
	listing << "<h1>Index of " << this->oldPath << "</h1><hr><ul>";

	if ((dir = opendir(this->path.c_str())) != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
			listing << "<li><a href=\"" << this->oldPath << entry->d_name << "\">" << entry->d_name << "</a></li>";
		closedir(dir);
	}

	listing << "</ul><hr></body></html>";

	this->body = listing.str();
	this->contentType = "text/html";
	theGetHeaderResponse(OK, CONTENT_LENGHT);
	this->response += this->body;
}

void Response::regularFileGet(void)
{
	std::string extension;
	std::map<std::string, std::string>::iterator mime_it;

	extension = getFileExtension(this->path);
	if (extension != "")
	{
		mime_it = this->files.mime.find(extension);
		if (mime_it != this->files.mime.end())
			this->contentType = mime_it->second;
	}
	else
		this->contentType = "applocation/octet-stream";

	if (!this->streamStart)
		this->inFile.open(this->path.c_str());

	if (!this->inFile.is_open())
		theGetErrorNotFound();
	else
		theGetResponseOk();

	if (this->request->getDoneServing())
		this->inFile.close();
}

// std::string	getStrTime()
// {

// }

void Response::theGetMethod(void)
{
	if (request->getError() != 0)
	{
		errorPage(request->getError());
		request->setDoneServing();
		return;
	}
	if (!this->gotTime)
	{
		tm *local_time;
		time_t now;

		now = time(0);
		local_time = localtime(&now);

		this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
		this->gotTime = true;
	}

	if (!this->dataCopy)
	{
		this->path = this->request->getNewPath();
		this->oldPath = this->request->getOldPath();
		this->dataCopy = true;
	}

	struct stat buffer;

	this->response.clear();
	this->redirection.clear();

	if (stat(this->path.c_str(), &buffer))
	{
		theGetErrorNotFound();
		this->request->setDoneServing();
	}
	else
	{
		if (S_ISDIR(buffer.st_mode))
		{
			if (this->path[this->path.size() - 1] != '/')
				theGetRedirectionRequest();
			else
				directoryListing();
			this->request->setDoneServing();
		}
		else if (!(buffer.st_mode & S_IRUSR))
		{
			theGetErrorForbidden();
			this->request->setDoneServing();
		}
		else
			regularFileGet();
	}
}
