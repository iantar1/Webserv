/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:58:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/20 02:12:10 by iantar           ###   ########.fr       */
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
	(void)path;
	return (1);
}

static const std::string	filePath[] = {"defaultPages/delete.htm", "delete.htm", "delete.htm"};

void Response::DeleteMethod()
{
	std::ifstream	file;
	std::stringstream buffer;
	// int	status = 0;

	// if (deleteChecking(uri))
	// 	status = DeleteDiractory(uri);
	file.open("defaultPages/delete.htm");
    buffer << file.rdbuf();
	response = buffer.str() + "\r\n\r\n";
	std::cout << "*********************************\n";
	// if (deleteChecking(path))
	// open file, 
	
	// getPageContent(std::string page);
}
