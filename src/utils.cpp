/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:58:44 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/05 16:57:39 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

size_t	find_last_of(const std::string& str, char ch)
{
	for (int i = str.length() - 1; i >= 0; --i) {
		if (str[i] == ch) {
			return i;
		}
	}
	return std::string::npos;
}

std::string dec_to_hex(int decimal)
{
	char buffer[50];

	sprintf(buffer, "%X", decimal);
	return std::string(buffer);
}

int	hexStringToInt(const std::string& hexStr)
{
	std::istringstream	iss(hexStr);
	int					result;

	iss >> std::hex >> result;
	return result;
}

std::string	getFileExtension(const std::string& fileName)
{
	size_t dotIndex = find_last_of(fileName, '.');
	if (dotIndex != std::string::npos) {
		return fileName.substr(dotIndex);
	}
	return "";
}

t_files	getDataFromFiles(void)
{
	t_files	data;

	data.mime = mimeTypes();
	data.headers = getHeaders();
	data.status = getStatus();
	return data;
}

std::map<int, std::string>	getHeaders(void)
{
	std::map<int, std::string>	headers;
	std::ifstream				inFile("/nfs/homes/iantar/Desktop/Webserv/Ressources/Headers");
	std::string					line;
	int							i = 0;

	if (!inFile.is_open())
		std::cout << "Couldn't open Headers file!" << std::endl;
	while (std::getline(inFile, line))
		headers.insert(std::make_pair(i++, line));
	return headers;
}

std::map<int, std::string>	getStatus(void)
{
	std::map<int, std::string>	status;
	std::ifstream				inFile("/nfs/homes/iantar/Desktop/Webserv/Ressources/Status");
	std::string					line;
	int							nb;

	if (!inFile.is_open())
		std::cout << "Couldn't open Status file!" << std::endl;
	while (std::getline(inFile, line))
	{
		nb = std::atoi(line.c_str());
		status.insert(std::make_pair(nb, line));
	}
	return status;
}

std::map<std::string, std::string> mimeTypes(void)
{
	std::map<std::string, std::string> out;
	std::ifstream inFile("/nfs/homes/iantar/Desktop/Webserv/Ressources/MIME.type");
	std::string line, key, value;
	int i;

	if (!inFile.is_open())
		std::cout << "Couldn't open MIME.type file!" << std::endl;
	while (std::getline(inFile, line))
	{
		key = "";
		value = "";
		i = 0;
		while ((line.c_str())[i] && (line.c_str())[i] != ' ')
			key += (line.c_str())[i++];
		i++;
		while ((line.c_str())[i] && (line.c_str())[i] != '\n')
			value += (line.c_str())[i++];
		out.insert(std::make_pair(key, value));
	}
	return out;
}

std::string	getContentExtension(std::map<std::string, std::string> mime, std::string contentType)
{
	std::map<std::string, std::string>::iterator	it;

	it = mime.begin();
	while (it != mime.end())
	{
		// std::cout << "{" << it->second << "}" << " {" << contentType << "}" << std::endl;
		if (it->second == contentType)
			return it->first;
		it++;
	}
	return "";
}

std::string	generateNameFile(void)
{
	std::string result;
	const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	const int charsetSize = sizeof(charset) - 1;

	for (int i = 0; i < 5; i++)
		result += charset[rand() % charsetSize];
	return result;
}

std::string	getPageContent(std::string page)
{
	std::ifstream	inFile(page.c_str());
	std::string		line, pageContent = "";

	if (inFile.is_open())
	{
		while (std::getline(inFile, line))
		{
			pageContent += line;
			pageContent += "\n";
		}
		inFile.close();
	}
	return pageContent;
}

bool	isAllowed(std::string &str)
{
	std::string	allowedChars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%");

	for (size_t i = 0; i < str.size(); i++)
	{
		if (allowedChars.find(str[i]) == std::string::npos)
			return false;
	}
	return true;
}

bool	isLongReq(std::string &str)
{
	if (str.size() > 2048)
		return true;
	return false;
}

bool	startsWith(std::string full, std::string start)
{
	if (!full.compare(0, start.size(), start))
		return true;
	return false;
}

std::string	replacePath(std::string path, std::string location, std::string root)
{
	path = root + path.substr(location.size());
	return path;
}
