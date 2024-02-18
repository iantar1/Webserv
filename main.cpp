/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:07:55 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/18 00:02:26 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/headers.hpp"

std::map<std::string, std::string> mimeTypes(void)
{
	std::map<std::string, std::string> out;
	std::ifstream inFile("MIME.type");
	std::string line, key, value;
	int i;

	if (!inFile.is_open())
		std::cout << "Couldn't open MIME.type file!" << std::cout;
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

std::string dec_to_hex(int decimal)
{
	char buffer[50];
	sprintf(buffer, "%X", decimal);
	return std::string(buffer);
}

int main(void)
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int server_add;
	int new_socket;

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

	std::string line, body, response;
	std::map<std::string, std::string> dictionary;
	std::map<std::string, std::string> mime = mimeTypes();

	while (true)
	{
		std::cout << "\n+++++++ Waiting for a new request ++++++++\n"
				  << std::endl;
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (new_socket < 0)
			return (std::cout << "accept failed" << std::endl, -1);

		char buffer[10000] = {0};
		int valread = read(new_socket, buffer, 10000);

		std::string str(buffer, valread);
		std::cout << str << std::endl;

		if (valread < 0)
			std::cout << "No bytes are there to read" << std::endl;

		Request request(buffer);
		std::cout << "1" << std::endl;
		request.checkMethod();
		std::cout << "2" << std::endl;
		Response response(new_socket, request, mime);
		std::cout << "3" << std::endl;
		std::cout << "------------------ Response sent -------------------\n"
				  << std::endl;
		close(new_socket);
	}
}
