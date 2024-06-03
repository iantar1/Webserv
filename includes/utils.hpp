/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:06:49 by nabboune          #+#    #+#             */
/*   Updated: 2024/05/27 18:58:50 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include <cctype>
#include <set>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "macros.hpp"

template <typename T>
std::string ToString(T num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}



typedef struct	s_files
{
	std::map<std::string, std::string>	mime;
	std::map<int, std::string>			headers;
	std::map<int, std::string>			status;
}				t_files;

std::map<std::string, std::string>	mimeTypes(void);
size_t	find_last_of(const std::string& str, char ch);
std::string	generateNameFile(std::string &str);

std::string	dec_to_hex(int decimal);
int	hexStringToInt(const std::string& hexStr);

// std::string	getPageContent(std::string page);
std::string	getFileExtension(const std::string& fileName);
std::string	replacePath(std::string path, std::string location, std::string root);
std::string	getContentExtension(std::map<std::string, std::string> mime, std::string contentType);

t_files	getDataFromFiles(void);
std::map<int, std::string>	getHeaders(void);
std::map<int, std::string>	getStatus(void);
std::map<std::string, std::string> mimeTypes(void);

bool	isAllowed(std::string &str);
bool	isLongReq(std::string &str);
bool	startsWith(std::string full, std::string start);


std::string toLower(const std::string&);

template <typename T>
std::string toStr(const T& value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

// parsing utils functions

std::string					removeExtraSpaces(std::string const& line);
std::vector<std::string>	split(std::string line, char delim);
bool 						isPosNumber(std::string const& str);
int 						stringToPosInt(std::string const& str);
bool 						checkIp(std::string ip);
bool 						checkEnable(std::string enable);
bool 						isBrace(std::string line);
bool						isNumeric(const std::string& str);
std::string					ipToString(unsigned char* ip);
std::string					getHostIP(std::string const& hostname);

std::string skipLeadingWhitespace(const std::string&);

bool	isDirectory(const std::string& path);
bool	endsWith(std::string full, std::string end);
std::map<int, std::string>	readFileIntoString(const std::string& filename);
std::string str_to_upper(const std::string &str);
std::string change_a_to_b(const std::string &str, char a, char b);
std::string buildCgiMetaVariables(const std::string &key, const std::string &value);


std::string	getPathFromUri(const std::string& uri, const std::string& locationName);