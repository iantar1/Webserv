/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:58:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/20 04:52:43 by iantar           ###   ########.fr       */
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

bool Response::deleteFile(const std::string &path)
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
			status = DeleteDiractory(entryFullPath.c_str());
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

// Not found
// permission denied

bool deleteChecking(const std::string &path)
{
	if (std::strncmp(path.c_str(), "/To_Delete/", 10) != 0)
		return (false);
	if (path.size() == 10)
		return (false);
	return (true);
}

static const std::string	filePath[] = {"delete.htm", "delete.htm", "delete.htm"};

void Response::DeleteMethod()
{
	std::ifstream	file;
	std::stringstream buffer;
	int	status = 0;

	std::cout<< RED << uri<< RESET << "\n";
	// exit(1);
	if (deleteChecking(uri) == false)
		return ;
	if (isFile(uri))
	{
		status = deleteFile(uri);
	}
	else
	{
		status = DeleteDiractory(request->getNewPath());
	}
	std::cout << "this->path: "<< request->getNewPath() << "\n";
	// std::strin
	file.open((std::string("defaultPages/") + filePath[status]).c_str());
    buffer << file.rdbuf();
	response = buffer.str() + "\r\n\r\n";
}
