/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:07:55 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/12 22:46:41 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fstream>
#include <unistd.h>

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

int	main(void)
{
	int	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int	server_add;
	int	new_socket;

	if (server_fd < 0)
		return (std::cout << "cannot create socket" << std::endl, -1);
	std::cout << server_fd << std::endl;

	struct sockaddr_in address;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_add = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	if (server_add < 0)
		return (std::cout << "bind failed" << std::endl, -1);
	std::cout << server_add << std::endl;
	
	int addrlen = sizeof(address);

	if (listen(server_fd, 3) < 0)
		return (std::cout << "listen failed" << std::endl, -1);

	std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	char	*file = NULL;
	while(true)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (new_socket < 0)
			return (std::cout << "accept failed" << std::endl, -1);

		char buffer[2048] = {0};
		int valread = read(new_socket, buffer, 2048);

		// std::cout << buffer << std::endl;
		file = parse(buffer);
		std::cout << "FILE : " << file << std::endl;

		if(valread < 0)
			std::cout << "No bytes are there to read" << std::endl;
		write(new_socket, hello.c_str(), hello.size());
		std::cout << "------------------Hello message sent-------------------\n" << std::endl;
		close(new_socket);
	}
}
