/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:56:53 by nabboune          #+#    #+#             */
/*   Updated: 2024/04/06 13:50:06 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

void Response::theGetHeaderResponse(int code, int transferType)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(code)->second;

	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.find(CONTENT_TYPE);
	if (this->contentType_cgi.empty())
		header_it->second += this->contentType;
	else
		header_it->second += this->contentType_cgi;

	if (!this->cookie.empty()) {
		header_it = this->files.headers.find(SET_COOKIE);
		header_it->second += this->cookie;
	}
	else
		this->files.headers.erase(SET_COOKIE);

	if (!this->redirection.empty())
	{
		header_it = this->files.headers.find(LOCATION);
		header_it->second += this->redirection;
	}
	else
		this->files.headers.erase(LOCATION);

	if (transferType == CONTENT_LENGHT)
	{
		header_it = this->files.headers.find(CONTENT_LENGHT);
		header_it->second += ToString(this->body.size());
	}

	//  override your map here
	
	header_it = this->files.headers.begin();
	while (header_it != this->files.headers.end())
	{
		if ((transferType == TRANSFER_ENCODING && header_it->first != CONTENT_LENGHT)
			|| (transferType == CONTENT_LENGHT && header_it->first != TRANSFER_ENCODING))
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

void Response::theGetRedirectionIndex(std::string newIndex)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/301.htm") + "\r\n\r\n";
	this->redirection = newIndex;
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
	{
		std::cout << "CLOSE\n";
		this->inFile.close();
	}
}

bool	Response::checkPreGetMethod(void)
{
	if (request->getError() != 0)
	{
		errorPage(request->getError());
		request->setDoneServing();
		return false;
	}

	if (!this->request->getBody().empty())
	{
		errorPage(BAD_REQUEST);
		request->setDoneServing();
		return false;
	}

	if (!this->gotTime)
	{
		tm *local_time;
		time_t now;

		now = time(0);
		local_time = localtime(&now);

		this->strTime = ToString(local_time->tm_year + 1900)
						+ "-" + ToString(local_time->tm_mon + 1)
						+ "-" + ToString(local_time->tm_mday)
						+ " " + ToString(local_time->tm_hour)
						+ ":" + ToString(local_time->tm_min)
						+ ":" + ToString(local_time->tm_sec);
		this->gotTime = true;
	}

	if (!this->dataCopy)
	{
		this->path = this->request->getNewPath();
		this->oldPath = this->request->getOldPath();
		this->dataCopy = true;
	}

	if (stat(this->path.c_str(), &this->buffer) == -1)
	{
		theGetErrorNotFound();
		this->request->setDoneServing();
		return false;
	}
	return true;
}

void Response::theGetMethod(void)
{
	std::cout << "PATH : " << this->path << std::endl;
	std::cout << "OLD PATH : " << this->oldPath << std::endl;

	this->response.clear();
	this->redirection.clear();

	if (S_ISDIR(this->buffer.st_mode))
	{
		if (this->path[this->path.size() - 1] != '/') {
			theGetRedirectionRequest();
			this->request->setDoneServing();
			return;
		}
		else
		{
			if (!this->request->location.getIndex().empty())
			{
				for (size_t i = 0; i < this->request->location.getIndex().size(); i++)
				{
					if (stat(this->request->location.getIndex().at(i).c_str(), &this->buf) == 0)
					{
						if (S_ISDIR(this->buf.st_mode))
							theGetRedirectionIndex(this->request->location.getIndex().at(i));
						else
							servPage(this->request->location.getIndex().at(i));
						this->request->setDoneServing();
						return ;
					}
					// if (!access(this->request->location.getIndex().at(i).c_str(), R_OK)) 
					// {
					// 	/*
					// 		==============================================
					// 		ISMAIL WORKS : CGI !!!
					// 		==============================================
					// 	*/
					// 	servPage(this->request->location.getIndex().at(i));
					// 	this->request->setDoneServing();
					// 	return;
					// }
				}
			}
			if (this->request->location.getAutoIndex())
				directoryListing();
			else
				theGetErrorForbidden();
		}
		this->request->setDoneServing();
	}
	if (!(this->buffer.st_mode & S_IRUSR))
	{
		theGetErrorForbidden();
		this->request->setDoneServing();
	}
	else
			regularFileGet();
}
