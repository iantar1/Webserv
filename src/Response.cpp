/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 14:35:30 by iantar            #+#    #+#             */
/*   Updated: 2024/03/13 14:56:28 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/macros.hpp"
# include "../includes/Response.hpp"
# include "../includes/utils.hpp"

Response::Response(Request* request, t_files &files) : contentTotalSizePosted(0),
		request(request), files(files), streamStart(false), outOpened(false), 
		gotTime(false), modeChecked(false), dataCopy(false), startedTheChunk(false)
{
	this->socket = request->getFdSocket();
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
	// write(this->socket, this->response.c_str(), this->response.size());
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
		// std::cout << "=========================================================\n";
		theGetMethod();
		// write(this->request->getFdSocket(), this->response.c_str(), this->response.size());
	}
	else if (request->getMethdType() == POST)
	{
		// thePostMethod(mode);
		PostResponse();
	}
	//else Delete
}
// ******** DELETE MEthod ************

bool	isFile(const std::string& path)
{
	struct stat file_info;

    if (stat(path.c_str(), &file_info))
        return (false);
    return (S_ISREG(file_info.st_mode));
}

bool	isDiractory(const std::string& path)
{
	struct stat file_info;

    if (stat(path.c_str(), &file_info))
        return (false);
    return (S_ISDIR(file_info.st_mode));
}

bool	deleteFile(const std::string& path)
{
	if (access(path.c_str(), W_OK))
        return (PERMISSION_DENIED);
    if (unlink(path.c_str()))
        return (ERROR);
    return (SUCCESS);
}

int	deleteDiractory(const std::string& path)
{
	struct dirent	*entry;
    DIR 			*dir_stream;
    std::string		entryFullPath;
	int 			status;

    dir_stream = opendir(path.c_str());
    if (dir_stream == NULL)
    {
        return (PERMISSION_DENIED);
    }
    if (access(path.c_str(), R_OK | W_OK))
    {
        closedir(dir_stream);
        return (PERMISSION_DENIED);
    }
    while ((entry = readdir(dir_stream)) != NULL)
    {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue ;
        entryFullPath = path;
        if (entryFullPath[entryFullPath.size() - 1] != '/')
            entryFullPath += '/';
        entryFullPath += entry->d_name;
        if (isDiractory(entryFullPath.c_str()))
        {
			status = deleteDiractory(entryFullPath.c_str());
            if (status)
				return (status); //! rmdir fail
        }
        if (isFile(entryFullPath.c_str()))
        {
			status = deleteFile(entryFullPath.c_str());
            if (status)
			{
                closedir(dir_stream);
				return (status);
			}
        }
    }
    closedir(dir_stream);
    return (rmdir(path.c_str()));
}


void	Response::DeleteMethod(const std::string& path)
{
	struct stat statBuf;

	checkAllowedDelete();
    if (stat(path.c_str(), &statBuf)) {
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
		std::cout << "####" << std::endl;
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
	if (!this->gotTime) 
	{
		tm				*local_time;
		time_t			now;

		now = time(0);
		local_time = localtime(&now);

		this->strTime = ToString(local_time->tm_year + 1900) + "-"
			+ ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday)
				+ " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min)
					+ ":" + ToString(local_time->tm_sec);
		this->gotTime = true;
	}

	if (!this->dataCopy)
	{
		this->path = this->request->getNewPath();
		this->oldPath = this->request->getOldPath();
		this->dataCopy = true;
	}

	struct stat		buffer;

	this->response.clear();
	this->redirection.clear();

	// std::cout << "body: " << this->request->getBody() << "\n";
	// std::cout << GREEN << "REQUEST OLD PATH:\n" << this->oldPath << "\nEND REQUEST" << RESET << std::endl;
	// std::cout << BLUE << "REQUEST NEW PATH:\n" << this->path << "\nEND REQUEST" << RESET << std::endl;
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























void	Response::PostResponse()
{
	// std::cout << YELLOW << this->request->getBody() << RESET << std::endl;
	// this->request->printRequest();
	// std::cout << RED << "ACHE HAD IKHAN : " << this->request->getRequest().find("Content-Length")->first << RESET << std::endl;
	if (!this->modeChecked)
	{
		if (this->request->getRequest().find("Content-Length") != this->request->getRequest().end())
			this->mode = NORMAL;
		else
			this->mode = CHUNKED;
		this->modeChecked = true;
	}
	thePostMethod();
}

void	Response::thePostMethod()
{
	std::string										extension, fileName;
	std::map<std::string, std::string>				mime;
	std::map<std::string, std::string>::iterator	mime_it;

	if (!this->gotTime)
	{
		tm												*local_time;
		time_t											now;

		now = time(0);
		local_time = localtime(&now);
		this->strTime = ToString(local_time->tm_year + 1900) + "-"
			+ ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday)
				+ " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min)
					+ ":" + ToString(local_time->tm_sec);
	}

	if (!this->dataCopy)
	{
		// std::cout << "SALAM O3ALAYKOUME!!!!!!!!!\n";
		// std::cout << *mode << std::endl;
		if (this->mode == NORMAL)
		{
			this->postType = NORMAL_POST;
			this->contentLenght = std::atoi((this->request->getRequest().find("Content-Length")->second).c_str());
		}
		else
			this->postType = CHUNKED_POST;
		this->dataCopy = true;
		this->contentType = this->request->getRequest().find("Content-Type")->second;

		extension = getContentExtension(this->files.mime, this->contentType);
		fileName = "Uploads/" + generateNameFile() + extension;

		this->response = "";
	}

	// std::cout << RED << "FILE NAME : " << fileName << RESET << std::endl;

	if (!this->outOpened) {
		this->outFile.open(fileName.c_str(), std::ios::app);
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
	// write(this->socket, this->response.c_str(), this->response.size());
	// std::cout << "WRITE\n";
}

void	Response::thePostResponseCreatedPage(void)
{
	this->contentType = "text/html";
	this->body = getPageContent("defaultPages/201.htm") + "\r\n\r\n";
	thePostHeaderResponse(CREATED, CONTENT_LENGHT);
	this->response += this->body;
	// write(this->socket, this->response.c_str(), this->response.size());
	// std::cout << "WRITE\n";
}

void	Response::thePostResponseCreate(void)
{
	// std::cout << "========||==========\n";
	// std::cout << this->request->getBody() << "\n";
	// std::cout << "========||==========\n";
	// this->request->printRequest();
	if (this->postType == NORMAL_POST)
	{
		this->outFile.write(this->request->getBody().data(), this->request->getBody().size());
		std::cout << YELLOW << "=================||=================" << RESET << std::endl;
		std::cout << RED << "SIZE: " << this->request->getBody().size() << RESET << "\nDATA:\n" << this->request->getBody().data() << std::endl;
		std::cout << YELLOW << "=================||=================" << RESET << std::endl;
		this->contentTotalSizePosted += this->request->getBody().size();
		std::cout << YELLOW << "=================_-_-_=================" << RESET << std::endl;
		std::cout << BLUE << this->contentTotalSizePosted << " || " << this->contentLenght << RESET << std::endl;
		std::cout << YELLOW << "=================_-_-_=================" << RESET << std::endl;
		// std::cout << RED << this->contentLenght << RESET << std::endl;
		// std::cout << this->contentTotalSizePosted << "\n";
		if (this->contentTotalSizePosted == this->contentLenght) {
			this->request->setDoneServing();
			thePostResponseCreatedPage();
			this->outFile.close();
			// std::cout << "||TT||TT||TT||TT||TT||TT||\n";
		}
	}
	else if (this->postType == CHUNKED_POST)
	{
		std::string		data = this->request->getBody();
		// int				restart;

		// if (!this->startedTheChunk) {
		// 	int	nl = this->request->getBody().find("\r\n");

		// 	if (nl != std::string::npos) {
		// 		this->ccl = hexStringToInt(this->request->getBody());
		// 		nl += 2;
		// 	}
		// 	else {
		// 		nl = 0;
		// 		restart = -1;
		// 	}
		// 	this->outFile.write(this->request->getBody().substr(nl).c_str(), this->request->getBody().substr(nl).size());
		// 	this->contentTotalSizePosted += this->request->getBody().substr(nl).size();
		// 	if (this->ccl != this->contentTotalSizePosted)
		// 		this->startedTheChunk = true;
		// 	else {
		// 		this->startedTheChunk = false;
		// 		this->contentTotalSizePosted = 0;
		// 	}
		// }
		// else {
		// 	int	nl = this->request->getBody().find_first_of("\r\n");
		// 	// int	lnl = this->request->getBody().find_first_of("\r\n");

		// 	if (nl == std::string::npos) {
		// 		this->outFile.write(this->request->getBody().c_str(), this->request->getBody().size());
		// 		this->contentTotalSizePosted += this->request->getBody().size();
		// 		restart = -1;
		// 	}
		// 	else {
		// 		this->outFile.write(this->request->getBody().substr(0, nl).c_str(), nl);
		// 		this->contentTotalSizePosted += nl;
		// 		restart = nl + 2;
		// 	}

		// 	if (this->ccl == this->contentTotalSizePosted) {
		// 		this->startedTheChunk = false;
		// 		this->contentTotalSizePosted = 0;
		// 	}
		// }

/*
	chunk :
			5
			Hello
			5
			 Worl
			5
			d, Ho
			10
			w are thin
			3
			gs?
*/























		this->ccl = hexStringToInt(this->request->getChunkedBodySize());
		// std::cout << ccl << " || " << this->request->getChunkedBodySize() << std::endl;
		if (this->ccl != 0)
			this->outFile.write(data.data(), ccl);
			// write(this->socket, this->request.getBody().data(), ccl);
		else
			this->mode = NORMAL;
	}
}

const std::string&		Response::getResponse() const { return this->response; }
