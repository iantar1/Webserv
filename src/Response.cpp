/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:09:09 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/19 01:07:50 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::Response(Request *request, t_files &files) : contentTotalSizePosted(0),
													   request(request), files(files), streamStart(false), outOpened(false),
													   gotTime(false), modeChecked(false), dataCopy(false), startedTheChunk(false)
{
	this->socket = request->getFdSocket();
}

Response::~Response(void)
{

	std::cout << GREEN << "RESPONCE DESTRUCTOR\n"
			  << RESET;
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
	this->response.clear();
}

void Response::StartResponse()
{
	// if (request->getError() != 0)
	// {
	// 	errorPage(request->getError());
	// 	return ;
	// }

	if (request->getMethdType() == GET)
	{
		// std::cout << "=========================================================\n";
		cgi_Handler("/nfs/homes/iantar/Desktop/Webserv/ph.php");
		theGetMethod();
		// write(this->request->getFdSocket(), this->response.c_str(), this->response.size());
	}
	else if (request->getMethdType() == POST)
	{
		// thePostMethod(mode);
		PostResponse();
	}
	else if (request->getMethdType() == DELETE)
	{
	}
}
// ******** DELETE MEthod ************

bool Response::isFile(const std::string &path)
{
	struct stat file_info;

	if (stat(path.c_str(), &file_info))
		return (false);
	return (S_ISREG(file_info.st_mode));
}

bool Response::isDiractory(const std::string &path)
{
	struct stat file_info;

	if (stat(path.c_str(), &file_info))
		return (false);
	return (S_ISDIR(file_info.st_mode));
}

bool Response::deleteFile(const std::string &path)
{
	if (access(path.c_str(), W_OK))
		return (PERMISSION_DENIED);
	if (unlink(path.c_str()))
		return (ERROR);
	return (SUCCESS);
}

int Response::DeleteMethod(const std::string &path)
{
	struct dirent *entry;
	DIR *dir_stream;
	std::string entryFullPath;
	int status;

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
			continue;
		entryFullPath = path;
		if (entryFullPath[entryFullPath.size() - 1] != '/')
			entryFullPath += '/';
		entryFullPath += entry->d_name;
		if (isDiractory(entryFullPath.c_str()))
		{
			status = DeleteMethod(entryFullPath.c_str());
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

// bool deleteChecking(const std::string &path)
// {

// }

// void Response::DeleteMethod(const std::string &path)
// {
// 	struct stat statBuf;

// 	if (deleteChecking(path))
// 		return ;

// }

const std::string &Response::getResponse() const { return this->response; }
