/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:06:49 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/20 21:46:13 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

typedef struct	s_files
{
	std::map<std::string, std::string>	mime;
	std::map<int, std::string>			headers;
	std::map<int, std::string>			status;
}				t_files;

std::string	dec_to_hex(int decimal);
std::map<std::string, std::string>	mimeTypes(void);
size_t	find_last_of(const std::string& str, char ch);
std::string	getFileExtension(const std::string& fileName);

t_files	getDataFromFiles(void);
std::map<int, std::string>	getHeaders(void);
std::map<int, std::string>	getStatus(void);
std::map<std::string, std::string> mimeTypes(void);

template <typename T>
std::string toStr(const T& value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}
