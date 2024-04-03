/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:58:44 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/29 00:12:28 by iantar           ###   ########.fr       */
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

std::map<std::string, std::string>	getHeaders(void)
{
	std::map<std::string, std::string>	headers;

	headers.insert(std::make_pair("VERSION", "HTTP/1.1"));
	headers.insert(std::make_pair("DATE", ""));
	headers.insert(std::make_pair("SERVER", "Tekshbila"));
	headers.insert(std::make_pair("CONTENT-TYPE", ""));
	headers.insert(std::make_pair("CONTENT-LENGHT", ""));
	headers.insert(std::make_pair("TRANSFER-ENCODING", "chunked"));
	headers.insert(std::make_pair("LOCATION", ""));
	headers.insert(std::make_pair("SET-COOKIE", ""));
	headers.insert(std::make_pair("CACHE-CONTROL", ""));

	return headers;
}

std::map<int, std::string>	getStatus(void)
{
	std::map<int, std::string>	status;

	status.insert(std::make_pair(200, " 200 OK"));
	status.insert(std::make_pair(201, " 201 Created"));
	status.insert(std::make_pair(202, " 202 Accepted"));
	status.insert(std::make_pair(204, " 204 No Content"));
	status.insert(std::make_pair(301, " 301 Moved Permanently"));
	status.insert(std::make_pair(302, " 302 Found"));
	status.insert(std::make_pair(303, " 303 See Other"));
	status.insert(std::make_pair(304, " 304 Not Modified"));
	status.insert(std::make_pair(400, " 400 Bad Request"));
	status.insert(std::make_pair(401, " 401 Unauthorized"));
	status.insert(std::make_pair(403, " 403 Forbidden"));
	status.insert(std::make_pair(404, " 404 Not Found"));
	status.insert(std::make_pair(405, " 405 Method Not Allowed"));
	status.insert(std::make_pair(406, " 406 Not Acceptable"));
	status.insert(std::make_pair(408, " 408 Request Timeout"));
	status.insert(std::make_pair(409, " 409 Conflict"));
	status.insert(std::make_pair(411, " 411 Length Required"));
	status.insert(std::make_pair(413, " 413 Payload Too Large"));
	status.insert(std::make_pair(414, " 414 URI Too Long"));
	status.insert(std::make_pair(415, " 415 Unsupported Media Type"));
	status.insert(std::make_pair(500, " 500 Internal Server Error"));
	status.insert(std::make_pair(501, " 501 Not Implemented"));
	status.insert(std::make_pair(502, " 502 Bad Gateway"));
	status.insert(std::make_pair(503, " 503 Service Unavailable"));
	status.insert(std::make_pair(505, " 505 HTTP Version Not Supported"));

	return status;
}

std::map<std::string, std::string> mimeTypes(void)
{
	std::map<std::string, std::string> out;
	std::ifstream inFile("DataBase/MIME.type");
	std::string line;
	std::vector<std::string> splitLine;
	int i;

	if (!inFile.is_open()) {
		std::cerr << "Couldn't open MIME.type file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (std::getline(inFile, line))
	{
		if(line.empty())
			continue;
		line = removeExtraSpaces(line);
		splitLine = split(line, SPACE);
		out.insert(std::make_pair(splitLine[0], splitLine[1]));
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
