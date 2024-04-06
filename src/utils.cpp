/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:58:44 by nabboune          #+#    #+#             */
/*   Updated: 2024/04/06 05:23:40 by nabboune         ###   ########.fr       */
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
	std::ifstream				inFile("Ressources/Headers");
	std::string					line;
	int							i = 0;

	if (!inFile.is_open()) {
		std::cerr << "Couldn't open Headers file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (std::getline(inFile, line))
		headers.insert(std::make_pair(i++, line));
	inFile.close();
	return headers;
}

std::map<int, std::string>	getStatus(void)
{
	std::map<int, std::string>	status;
	std::ifstream				inFile("Ressources/Status");
	std::string					line;
	int							nb;

	if (!inFile.is_open()) {
		std::cerr << "Couldn't open Status file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (std::getline(inFile, line))
	{
		nb = std::atoi(line.c_str());
		status.insert(std::make_pair(nb, line));
	}
	inFile.close();
	return status;
}

std::map<std::string, std::string> mimeTypes(void)
{
	std::map<std::string, std::string> out;
	std::ifstream inFile("Ressources/MIME.type");
	std::string line, key, value;
	int i;

	if (!inFile.is_open()) {
		std::cerr << "Couldn't open MIME.type file!" << std::endl;
		exit(EXIT_FAILURE);
	}
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
	inFile.close();
	return out;
}

std::string	getContentExtension(std::map<std::string, std::string> mime, std::string contentType)
{
	std::map<std::string, std::string>::iterator	it;

	it = mime.begin();
	while (it != mime.end())
	{
		if (it->second == contentType)
		{
			return it->first;
		}
		it++;
	}
	return "";
}

std::string	generateNameFile(std::string &str)
{
	std::string result = "file_" + str + "_";
	const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	const int charsetSize = sizeof(charset) - 1;

	for (int i = 0; i < 5; i++)
		result += charset[rand() % charsetSize];
	return result;
}

// std::string	getPageContent(std::string page)
// {
// 	// std::string		pgNbStr = (split(page, '/').back());
// 	// std::istringstream pg(pgNbStr);
// 	// int pgNb;
// 	// pg >> pgNb;
// 	std::ifstream	inFile(page.c_str());
// 	std::string		line, pageContent = "";
 
// 	if (inFile.is_open())
// 	{
// 		while (std::getline(inFile, line))
// 		{
// 			pageContent += line;
// 			pageContent += "\n";
// 		}
// 	}
// 	inFile.close();
// 	return pageContent;
// }

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

bool	endsWith(std::string full, std::string end)
{
	if (!full.compare(full.size() - end.size(), end.size(), end))
		return true;
	return false;
}

std::string	replacePath(std::string path, std::string location, std::string root)
{
	path = root + path.substr(location.size());
	return path;
}

std::string toLower(const std::string& str)
{
    std::string result;
    for (size_t i = 0; i < str.size(); i++) {
        result += std::tolower(str[i]);
    }
    return (result);
}

std::string skipLeadingWhitespace(const std::string &str)
{
	std::string result;
	size_t index;

	for (index = 0; index < str.length() && isspace(str[index]); ++index)
	{
	}
	return (str.substr(index));
}

bool	isDirectory(const std::string& path) {
	struct stat fileInfo;

	if (stat(path.c_str(), &fileInfo) != 0)
		return false;

	return S_ISDIR(fileInfo.st_mode);
}

std::map<int, std::string>	readFileIntoString(const std::string& filename)
{
	std::map<int, std::string>	stringMap;
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		stringMap[0] = "Error";
		return stringMap;
	}

	stringMap[0] = "Success";
	std::stringstream buffer;
	buffer << file.rdbuf();
	stringMap[1] = buffer.str();
	return stringMap;
}

bool	isNumeric(const std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}
