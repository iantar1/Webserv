/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:58:36 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 19:04:13 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

bool Response::isFile(const std::string& path)
{
	struct stat file_info;

	if (stat(path.c_str(), &file_info))
		return (false);
	return (S_ISREG(file_info.st_mode));
}

bool Response::isDiractory(const std::string& path)
{
	struct stat file_info;

	if (stat(path.c_str(), &file_info))
		return (false);
	return (S_ISDIR(file_info.st_mode));
}

int Response::deleteFile(const std::string& path)
{
	if (access(path.c_str(), W_OK))
		return (PERMISSION_DENIED);
	if (unlink(path.c_str()))
		return (ERROR);
	return (SUCCESS);
}

int Response::DeleteDiractory(const std::string& path)
{
	struct dirent* entry;
	DIR* dir_stream;
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
			// check if rmdir fails
			status = DeleteDiractory(entryFullPath.c_str());
			if (status)
				return (status);
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

static const std::string filePath[] = { "defaultPages/204.htm", "defaultPages/403.htm", "defaultPages/500.htm", "defaultPages/404.htm" };
static const int status_code[4] = { 204, 403, 500, 404 };

// * the main function

void Response::DeleteMethod()
{
	std::ifstream file;
	std::stringstream buffer;
	int status = 0;

	if (!this->gotTime)
	{
		tm* local_time;
		time_t now;

		now = time(0);
		local_time = localtime(&now);
		// ! add the snake line in a method 
		this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
		this->gotTime = true;
	}
	if (access(request->getNewPath().c_str(), F_OK))
	{
		status = NOT_EXIST;
	}
	else if (isFile(request->getNewPath()))
	{
		status = deleteFile(request->getNewPath());
	}
	else
	{
		status = DeleteDiractory(request->getNewPath());
		if (status < 0)
			status = 1;
	}
	this->body = getPageContent(filePath[status]) + "\r\n\r\n";
	this->contentType = "text/html";
	theDeleteHeaderResponse(status_code[status], CONTENT_LENGHT);
	this->response += this->body;
	request->setDoneServing();//! this must change to responseDone var, in Response class not in Request (OOP)
}

void Response::theDeleteHeaderResponse(int code, int transferType)
{
	std::map<int, std::string>::iterator header_it;

	if (this->doneCGI) {
		std::map<std::string, std::string>::iterator it; // = this->cgiResponseHeaders.find("STATUS");

		// if (it != this->cgiResponseHeaders.end())
		// 	this->response += "HTTP/1.1 " + it->second + "\r\n";
		// else if (code == 504)
		// 	this->response += "HTTP/1.1 504 TIMEOUT\r\n";
		// else
		this->response += "HTTP/1.1 200 OK\r\n";

		it = this->cgiResponseHeaders.find("CONTENT-LENGTH");
		if (it->second == "") {
			this->cgiResponseHeaders.erase(it);
			this->cgiResponseHeaders["TRANSFER-ENCODING"] = "chunked";
			this->cgiContentLength = false;
		}
		else
		{
			if (isNumeric(it->second))
				this->cgiContentLength = true;
			else {
				errorPage(BAD_GATEWAY);
				this->responseDone = true;
				return;
			}
		}

		it = this->cgiResponseHeaders.begin();
		while (it != this->cgiResponseHeaders.end())
		{
			if (str_to_upper(it->first) == "STATUS") // || str_to_upper(it->first) == "CONTENT-LENGTH"
			{
				it++;
				continue;
			}
			else
				this->response += it->first + ": " + it->second + "\r\n";
			it++;
		}

		this->response += "\r\n";
	}
	else  {
		if (!this->secondLoop)
		{
			header_it = this->files.headers.find(RESPONSE_STATUS);
			header_it->second += this->files.status.find(code)->second;

			header_it = this->files.headers.find(DATE);
			header_it->second += this->strTime;

			header_it = this->files.headers.find(CONTENT_TYPE);
			if (this->contentType_cgi.empty())
				header_it->second += this->contentType;
			else
				header_it->second += this->contentType_cgi;

			if (!this->cookie.empty())
			{
				header_it = this->files.headers.find(SET_COOKIE);
				header_it->second += this->cookie;
			}
			else
				this->files.headers.erase(SET_COOKIE);

			// return f l config file
			if (code == MOVED_PERMA && !this->request->getLocation().getReturnPath().empty())
				this->redirection = this->request->getLocation().getReturnPath();

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
				if ((transferType == TRANSFER_ENCODING && header_it->first != CONTENT_LENGHT) || (transferType == CONTENT_LENGHT && header_it->first != TRANSFER_ENCODING))
					this->response += header_it->second + "\r\n";
				header_it++;
			}
			this->response += "\r\n";
			this->secondLoop = true;
		}
	}
}
