/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:07:55 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 03:37:02 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Webserv.hpp"

// std::map<std::string, std::string>	requestParse(char *buffer)
// {
// 	int									i = 0;
// 	std::string							key, value;
// 	std::map<std::string, std::string>	output;
// 	std::map<std::string, std::string>::iterator	it;

// 	key = "";
// 	value = "";
// 	while(buffer[i] && buffer[i] != ' ')
// 		key += buffer[i++];
// 	i += 2;
// 	while(buffer[i] && buffer[i] != ' ' && buffer[i] != '\n')
// 		value += buffer[i++];
// 	while(buffer[i] && buffer[i] != '\n')
// 		i++;
// 	i++;
// 	output.insert(std::make_pair(key, value));
// 	while(buffer[i])
// 	{
// 		key = "";
// 		value = "";
// 		while(buffer[i] && buffer[i] != '\n')
// 		{
// 			if(buffer[i] != ':')
// 			{
// 				key += buffer[i++];
// 				continue;
// 			}
// 			else
// 			{
// 				i += 2;
// 				while(buffer[i] && buffer[i] != '\n')
// 					value += buffer[i++];
// 			}
// 		}
// 		i++;
// 		if (key != "" && value != "")
// 			output.insert(std::make_pair(key, value));
// 	}
// 	return (output);
// }

// std::map<std::string, std::string>::iterator	easyfind(std::map<std::string, std::string> &container, std::string x)
// {
// 	std::map<std::string, std::string>::iterator	it = container.begin();
// 	while (it != container.end())
// 	{
// 		if (it->first == x)
// 			return it;
// 		it++;
// 	}
// 	return it;
// }

std::map<std::string, std::string>	mimeTypes(void)
{
	std::map<std::string, std::string>	out;
	std::ifstream						inFile("MIME.type");
	std::string							line, key, value;
	int									i;

	if (!inFile.is_open())
		std::cout << "Couldn't open MIME.type file!" << std::cout;
	while (std::getline(inFile, line))
	{
		key = "";
		value = "";
		i = 0;
		while((line.c_str())[i] && (line.c_str())[i] != ' ')
			key += (line.c_str())[i++];
		i++;
		while((line.c_str())[i] && (line.c_str())[i] != '\n')
			value += (line.c_str())[i++];
		out.insert(std::make_pair(key, value));
	}
	return out;
}

std::string dec_to_hex(int decimal) {
    char	buffer[50];
    sprintf(buffer, "%X", decimal);
    return std::string(buffer);
}

// void	constructResponse(int socket, std::map<std::string, std::string> dictionary, std::map<std::string, std::string> mime)
// {
// 	std::map<std::string, std::string>::iterator	it = dictionary.begin();
// 	std::map<std::string, std::string>::iterator	response_it, mime_it;
// 	std::map<std::string, std::string>				response;
// 	std::stringstream								chunkStream;
// 	std::string										header, body, line, out;

// 	response.insert(std::make_pair("0", "HTTP/1.1 "));
// 	response.insert(std::make_pair("1", "Content-Type: "));
// 	// response.insert(std::make_pair("2", "Content-Length: "));
// 	response.insert(std::make_pair("2", "Transfer-Encoding: "));
// 	response.insert(std::make_pair("3", "Date: "));
// 	response.insert(std::make_pair("4", "Server: Muguiwarra"));
// 	while(it != dictionary.end())
// 	{
// 		if(it->first == "GET" || it->first == "POST" || it->first == "DELETE")
// 			break;
// 		it++;
// 	}
// 	if(it != dictionary.end() && it->first == "GET")
// 	{
// 		time_t	now = time(0);
// 		std::ifstream	inFile((it->second).c_str());
// 		mime_it = easyfind(mime, it->second);
		
// 		/*
// 			Get the extension of the file and then save the content type
// 		*/
		
// 		// std::cout << mime_it->first << " || " << mime_it->second << std::endl;
// 		// std::cout << it->second << std::endl;
// 		std::string		contentType = mime_it->second;
// 		// std::cout << contentType << std::endl;
// 		tm	*local_time = localtime(&now);
// 		std::string	strTime = ToString(local_time->tm_year + 1900) + "-" + ToString(local_time->tm_mon + 1) + "-" + ToString(local_time->tm_mday) + " " + ToString(local_time->tm_hour) + ":" + ToString(local_time->tm_min) + ":" + ToString(local_time->tm_sec);
// 		header = "";
// 		if (!inFile.is_open())
// 		{
// 			response_it = easyfind(response, "0");
// 			response_it->second += "404 Not Found";
// 			response_it = easyfind(response, "1");
// 			response_it->second += "text/plain";
// 			response_it = easyfind(response, "2");
// 			// response_it->second += ToString(body.size());
// 			response_it->second += "chunked";
// 			response_it = easyfind(response, "3");
// 			response_it->second += strTime;

// 			response_it = response.begin();
// 			while (response_it != response.end())
// 			{
// 				header += response_it->second + "\r\n";
// 				response_it++;
// 			}
// 			write(socket, header.c_str(), header.size());

// 			line = "Error 404!";
// 			body = dec_to_hex(line.size()) + "\r\n" + line + "\r\n";
// 			write(socket, body.c_str(), body.size());
// 		}
// 		else
// 		{
// 			response_it = easyfind(response, "0");
// 			response_it->second += "200 OK";
// 			response_it = easyfind(response, "1");
// 			response_it->second += contentType;
// 			response_it = easyfind(response, "2");
// 			// response_it->second += ToString(body.size());
// 			response_it->second += "chunked";
// 			response_it = easyfind(response, "3");
// 			response_it->second += strTime;

// 			response_it = response.begin();
// 			while (response_it != response.end())
// 			{
// 				header += response_it->second + "\r\n";
// 				response_it++;
// 			}
// 			write(socket, header.c_str(), header.size());

// 			std::cout << "HEADER : \n" << header << "\n====" <<std::endl;

// 			body = "";
// 			while (std::getline(inFile, line))
// 			{
// 				line = line + "\n";
// 				body = dec_to_hex(line.size()) + "\r\n" + line + "\r\n";				
// 				write(socket, body.c_str(), body.size());
// 			}
// 		}
// 		write(socket, "0\r\n\r\n", 5);
// 	}
// }

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

	std::string	line, body, response;
	std::map<std::string, std::string>	dictionary;
	std::map<std::string, std::string>	mime = mimeTypes();
	
	while(true)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0)
			return (std::cout << "accept failed" << std::endl, -1);

		char buffer[4096] = {0};
		int valread = read(new_socket, buffer, 4096);

		std::cout << buffer << std::endl;

		if(valread < 0)
			std::cout << "No bytes are there to read" << std::endl;

		Request	request(buffer);
		std::cout << "1" << std::endl;
		request.checkMethod();
		std::cout << "2" << std::endl;
		Response	response(new_socket, request, mime);
		std::cout << "3" << std::endl;
		// constructResponse(new_socket, dictionary, mime);

		// std::cout << response.c_str() << "{"<<response.size()<<"}" << std::endl;
		// write(new_socket, response.c_str(), response.size());
		std::cout << "------------------Hello message sent-------------------\n" << std::endl;
		close(new_socket);
	}
}
