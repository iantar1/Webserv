/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:07:55 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/13 23:41:15 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <map>

#define	PORT 8080

char	*parse(char *src)
{
	int	i = 0;
	int	j = 0;
	char	*temp;
	char	*dest;

	temp = src + 5;
	while(temp[i])
	{
		if (temp[i] == ' ')
			break;
		i++;
	}
	dest = new char[i + 1];
	while(j < i)
	{
		dest[j] = temp[j];
		j++;
	}
	dest[j] = '\0';
	return dest;
}

std::string ToString(std::size_t num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}

std::map<std::string, std::string>	requestParse(char *buffer)
{
	int									i = 0;
	std::string							key, value;
	std::map<std::string, std::string>	output;

	key = "";
	value = "";
	while(buffer[i] && buffer[i] != ' ')
		key += buffer[i++];
	i++;
	while(buffer[i] && buffer[i] != ' ' && buffer[i] != '\n')
		value += buffer[i++];
	output.insert(std::make_pair(key, value));
	while(buffer[i])
	{
		key = "";
		value = "";
		while(buffer[i] && buffer[i] != '\n')
		{
			if(buffer[i] != ':')
			{
				key += buffer[i++];
				continue;
			}
			else
			{
				i += 2;
				while(buffer[i] && buffer[i] != '\n')
					value += buffer[i++];
			}
		}
		i++;
		output.insert(std::make_pair(key, value));
	}
	return (output);
}

std::string	getContentType(std::string file)
{
	std::string	out;
	std::size_t	point = file.find('.');

	if(point == std::string::npos)
	{
		out = "text/plain";
		return (out);
	}
	// Get content type ".jpg => Content-Type: Image/jpg"
}

std::string	constructRequestHeader(std::map<std::string, std::string> dictionary)
{
	std::map<std::string, std::string>::iterator	it = map.begin();
	std::string										header, body, line;

	if(it->first = "GET")
	{
		std::ifstream	inFile(it->second);
		if (!inFile.is_open())
			header = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 10\n\nError 404!";
		else
		{
			body = "";
			while (std::getline(inFile, line))
				body += line + "\n";
			header = "HTTP/1.1 200 OK\nContent-Type: " + ?? + ToString(body.size()) + "\n\n" + body;
			// Construct the response header
		}

	}
}

int	main(void)
{
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int	server_add;
	int	new_socket;

	if (server_fd < 0)
		return (std::cout << "cannot create socket" << std::endl, -1);

	struct sockaddr_in address;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	int man3rf = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &man3rf, sizeof(int));

	server_add = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	if (server_add < 0)
		return (std::cout << "bind failed" << std::endl, -1);

	int addrlen = sizeof(address);

	if (listen(server_fd, 3) < 0)
		return (std::cout << "listen failed" << std::endl, -1);

	std::string	okHeaderText = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
	std::string	error = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 10\n\nError 404!";

	std::string	line, body, response;
	char	*file = NULL;
	std::map	dictionary;
	while(true)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0)
			return (std::cout << "accept failed" << std::endl, -1);

		char buffer[4096] = {0};
		int valread = read(new_socket, buffer, 4096);

		std::cout << buffer << std::endl;
		file = parse(buffer);
		std::cout << "FILE : " << file << std::endl;

		if(valread < 0)
			std::cout << "No bytes are there to read" << std::endl;

		std::ifstream	inFile(file);
		if (!inFile.is_open())
			write(new_socket, error.c_str(), error.size());
		else
		{
			body = "";
			while (std::getline(inFile, line))
				body += line + "\n";
			response = okHeaderText + ToString(body.size()) + "\n\n" + body;
		}
		std::cout << response.c_str() << "{"<<response.size()<<"}" << std::endl;
		write(new_socket, response.c_str(), response.size());
		std::cout << "------------------Hello message sent-------------------\n" << std::endl;
		close(new_socket);
	}
}
