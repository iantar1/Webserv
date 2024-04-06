/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:58:36 by iantar            #+#    #+#             */
/*   Updated: 2024/04/06 04:32:12 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

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

int Response::deleteFile(const std::string &path)
{
	if (access(path.c_str(), W_OK))
		return (PERMISSION_DENIED);
	if (unlink(path.c_str()))
		return (ERROR);
	return (SUCCESS);
}

int Response::DeleteDiractory(const std::string &path)
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

static const std::string filePath[] = {"defaultPages/204.htm", "defaultPages/403.htm", "defaultPages/500.htm", "defaultPages/404.htm"};
static const int status_code[4] = {204, 403, 500, 404};

void Response::DeleteMethod()
{
	std::ifstream file;
	std::stringstream buffer;
	int status  = 0;

	if (!this->gotTime)
	{
		tm *local_time;
		time_t now;

		now = time(0);
		local_time = localtime(&now);

		this->strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
		this->gotTime = true;
	}
	if (access(request->getNewPath().c_str(), F_OK))
	{
		status = NOT_EXIST;
		// request->se
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
	if (status_code[status] != 204)
		this->body = getPageContent(filePath[status]) + "\r\n\r\n";
	theDeleteHeaderResponse(status_code[status], CONTENT_LENGHT);
	this->contentType = "text/html";
	this->response += this->body;
}

void Response::theDeleteHeaderResponse(int code, int transferType)
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
		if ((transferType == TRANSFER_ENCODING && header_it->first != CONTENT_LENGHT) || (transferType == CONTENT_LENGHT && header_it->first != TRANSFER_ENCODING))
		{
			this->response += header_it->second + "\r\n";
		}
		header_it++;
	}
	this->response += "\r\n";
}
