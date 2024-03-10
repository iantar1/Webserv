/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 01:05:52 by nabboune          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/10 14:20:40 by nabboune         ###   ########.fr       */
=======
/*   Updated: 2024/03/10 13:26:34 by iantar           ###   ########.fr       */
>>>>>>> 564db27226c4e4db23988718a116265f3e1aa070
/*                                                                            */
/* ************************************************************************** */

# include "../includes/macros.hpp"
# include "../includes/PostResponse.hpp"
# include "../includes/Response.hpp"
# include "../includes/utils.hpp"

Response::Response(Request* request, t_files files) : streamStart(false)
{

	this->request = request;
	this->socket = request->getFdSocket();
	this->files = files;
}

Response::~Response(void)
{
	
	std::cout << GREEN << "RESPONCE DESTRUCTOR\n" << RESET;
}

void	Response::errorPage(int errorCode)
{
	std::map<int, std::string>::iterator	header_it;

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

void	Response::StartResponse()
{
	// if (request->getError() != 0)
	// {
	// 	errorPage(request->getError());
	// 	return ;
	// }

	if (request->getMethdType() == GET)
	{
		std::cout << "=========================================================\n";
		theGetMethod();
		write(this->request->getFdSocket(), this->response.c_str(), this->response.size());
	}
	// else if (request->getMethdType() == POST)
	// 	PostResponse	post(this->request->getFdSocket(), this->request, this->files);
    //else Delete
}
// ******** DELETE MEthod ************

void	Response::Delete(const std::string& path)
{
	struct stat statBuf;

	checkAllowedDelete();
    if (stat(path.c_str(), &statBuf)) {
		// ! set a flag in request file
		// ! you must check errors in requst 
		// ! and send what you want to delete or a flag
		if (check_flag)
		else
			CALL_method();
        throw std::runtime_error("");
    }
	
}

void		Response::theGetHeaderResponse(int code, int transferType)
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

void Response::theGetRedirectionRequest(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/301.htm") + "\r\n\r\n";
	this->redirection = this->oldPath + "/";
	// std::cout << this->redirection << "ooooo\n";
	theGetHeaderResponse(MOVED_PERMA, CONTENT_LENGHT);
	this->response += this->body;
	// write(this->request->getFdSocket(), this->response.c_str(), this->response.size());
}

void Response::theGetErrorBadRequest(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/400.htm") + "\r\n\r\n";
	theGetHeaderResponse(BAD_REQUEST, CONTENT_LENGHT);
	this->response += this->body;
	// write(this->socket, this->response.c_str(), this->response.size());
}

void Response::theGetErrorNotFound(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/404.htm") + "\r\n\r\n";
	theGetHeaderResponse(NOT_FOUND, CONTENT_LENGHT);
	this->response += this->body;
	// write(this->socket, this->response.c_str(), this->response.size());
}

void Response::theGetErrorForbidden(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/403.htm") + "\r\n\r\n";
	theGetHeaderResponse(FORBIDDEN, CONTENT_LENGHT);
	this->response += this->body;
	// write(this->socket, this->response.c_str(), this->response.size());
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
		if (byteRead <= 0) {
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

void			Response::directoryListing(void)
{
	DIR*				dir;
	struct dirent*		entry;
	std::ostringstream	listing;

	listing << "<html><head><title>Index of " << this->oldPath << "</title></head><body>";
	listing << "<h1>Index of " << this->oldPath << "</h1><hr><ul>";

	if ((dir = opendir(this->path.c_str())) != NULL) {
		while ((entry = readdir(dir)) != NULL) {
			// std::cout << this->oldPath << std::endl;
			listing << "<li><a href=\"" << this->oldPath << entry->d_name << "\">" << entry->d_name << "</a></li>";
		}
		closedir(dir);
	}

	listing << "</ul><hr></body></html>";

	this->body = listing.str();
	// std::cout << this->body << std::endl;
	this->contentType = "text/html";
	theGetHeaderResponse(OK, CONTENT_LENGHT);
	this->response += this->body;

	// write(this->socket, this->response.c_str(), this->response.size());
}

void			Response::regularFileGet(void)
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

		if (!this->streamStart)
			this->inFile.open(this->path.c_str());

		if (!this->inFile.is_open())
			theGetErrorNotFound();
		else
			theGetResponseOk();

		// std::cout << this->response << std::endl;
		if (this->request->getDoneServing())
			this->inFile.close();
}

void Response::theGetMethod(void)
{
	tm				*local_time;
	time_t			now;
	struct stat		buffer;

	now = time(0);
	this->path = this->request->getNewPath();
	this->oldPath = this->request->getOldPath();
	local_time = localtime(&now);

	this->strTime = ToString(local_time->tm_year + 1900) + "-"
		+ ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday)
			+ " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min)
				+ ":" + ToString(local_time->tm_sec);

	this->response.clear();
	this->redirection.clear();

	// std::cout << "body: " << this->request->getBody() << "\n";
	std::cout << GREEN << "REQUEST OLD PATH:\n" << this->oldPath << "\nEND REQUEST" << RESET << std::endl;
	std::cout << BLUE << "REQUEST NEW PATH:\n" << this->path << "\nEND REQUEST" << RESET << std::endl;
	if (this->request->getBody() != "")
		theGetErrorBadRequest();
	else if (stat(this->path.c_str(), &buffer))
	{
		theGetErrorNotFound();
	}
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
