/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 03:21:39 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/02 21:47:53 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/GetResponse.hpp"
#include "../includes/utils.hpp"

GetResponse::GetResponseException::GetResponseException(std::string error) : error(error) {}

const char *GetResponse::GetResponseException::what(void) const throw() { return this->error.c_str(); }

GetResponse::GetResponseException::~GetResponseException(void) throw() {}


GetResponse::GetResponse(int socket, Request &request, t_files files)
{
	this->request = request;
	this->socket = socket;
	this->files = files;


	theGetMethod();
}

GetResponse::GetResponse(const GetResponse &other) : request(other.request), files(other.files), socket(other.socket) {}

GetResponse &GetResponse::operator=(const GetResponse &other)
{
	this->request = other.request;
	this->socket = other.socket;
	this->files = other.files;
	return (*this);
}

GetResponse::~GetResponse(void) {}

void		GetResponse::theGetHeaderResponse(int code, int transferType)
{
	std::map<int, std::string>::iterator header_it;

	header_it = this->files.headers.find(RESPONSE_STATUS);
	header_it->second += this->files.status.find(code)->second;

	header_it = this->files.headers.find(DATE);
	header_it->second += this->strTime;

	header_it = this->files.headers.find(CONTENT_TYPE);
	header_it->second += this->contentType;

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

void GetResponse::theGetRedirectionRequest(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/301.htm") + "\r\n\r\n";
	this->redirection = this->path + "/";
	theGetHeaderResponse(MOVED_PERMA, CONTENT_LENGHT);
	this->response += this->body;
	write(this->socket, this->response.c_str(), this->response.size());
}

void GetResponse::theGetErrorBadRequest(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/400.htm") + "\r\n\r\n";
	theGetHeaderResponse(BAD_REQUEST, CONTENT_LENGHT);
	this->response += this->body;
	write(this->socket, this->response.c_str(), this->response.size());
}

void GetResponse::theGetErrorNotFound(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/404.htm") + "\r\n\r\n";
	theGetHeaderResponse(NOT_FOUND, CONTENT_LENGHT);
	this->response += this->body;
	write(this->socket, this->response.c_str(), this->response.size());
}

void GetResponse::theGetErrorForbidden(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/403.htm") + "\r\n\r\n";
	theGetHeaderResponse(FORBIDDEN, CONTENT_LENGHT);
	this->response += this->body;
	write(this->socket, this->response.c_str(), this->response.size());
}

void GetResponse::theGetResponseOk(void)
{
	theGetHeaderResponse(OK, TRANSFER_ENCODING);
	write(this->socket, this->response.c_str(), this->response.size());

	this->body = "";
	char buf[1025];
	std::streamsize byteRead = 1;

	while (byteRead > 0)
	{
		signal(SIGPIPE, SIG_IGN);
		this->inFile.read(buf, 1024);
		byteRead = this->inFile.gcount();
		if (byteRead <= 0)
			break;
		std::string str(buf, byteRead);

		this->body = dec_to_hex(str.size()) + "\r\n" + str + "\r\n";
		write(this->socket, this->body.c_str(), this->body.length());
	}

	write(this->socket, "0\r\n\r\n", 5);
}

void			GetResponse::directoryListing(void)
{
	DIR*				dir;
	struct dirent*		entry;
	std::ostringstream	listing;

	listing << "<html><head><title>Index of " << this->path << "</title></head><body>";
	listing << "<h1>Index of " << this->path << "</h1><hr><ul>";

	if ((dir = opendir(this->path.c_str() + 1)) != NULL) {
		while ((entry = readdir(dir)) != NULL) {
			listing << "<li><a href=\"" << this->path << entry->d_name << "\">" << entry->d_name << "</a></li>";
		}
		closedir(dir);
	}

	listing << "</ul><hr></body></html>";

	this->body = listing.str();
	this->contentType = "text/html";
	theGetHeaderResponse(OK, CONTENT_LENGHT);
	this->response += this->body;

	write(this->socket, this->response.c_str(), this->response.size());
}

void			GetResponse::regularFileGet(void)
{
		std::string										extension;
		std::map<std::string, std::string>::iterator	mime_it;

		extension = getFileExtension(this->path);
		if (extension != "")
		{
			mime_it = this->files.mime.find(extension);
			if (mime_it != this->files.mime.end())
				this->contentType = mime_it->second;
		}
		else
			this->contentType = "applocation/octet-stream";

		this->inFile.open(this->path.c_str() + 1);

		if (!this->inFile.is_open())
			theGetErrorNotFound();
		else
			theGetResponseOk();

		std::cout << this->response << std::endl;

		this->inFile.close();
}

void GetResponse::theGetMethod(void)
{
	tm				*local_time;
	time_t			now;
	struct stat		buffer;

	now = time(0);
	// this->path = this->request.getMethod().find("Path")->second;// ! what is this
	this->path = "";
	local_time = localtime(&now);
	this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
	this->response = "";
	this->redirection = "";

	if (this->request.getBody() != "")
		theGetErrorBadRequest();
	else if (stat(this->path.c_str() + 1, &buffer))
		theGetErrorNotFound();
	else
	{
		if (S_ISDIR(buffer.st_mode))
		{
			if (this->path[this->path.size() - 1] != '/')
				theGetRedirectionRequest();
			else
				directoryListing();
		}
		else if (!(buffer.st_mode & S_IRUSR))
			theGetErrorForbidden();
		else
			regularFileGet();
	}
}
