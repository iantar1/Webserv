/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:58:44 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/20 23:10:09 by nabboune         ###   ########.fr       */
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

	while (std::getline(inFile, line))
		headers.insert(std::make_pair(i++, line));
	return headers;
}

std::map<int, std::string>	getStatus(void)
{
	std::map<int, std::string>	status;
	std::ifstream				inFile("Ressources/Status");
	std::string					line;
	int							nb;

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
	std::ifstream inFile("Ressources/MIME.type");
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
