/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:06:49 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/17 00:45:55 by nabboune         ###   ########.fr       */
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

std::string	getPageContent(std::string page);
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
