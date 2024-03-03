/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 15:07:55 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/03 04:51:25 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/headers.hpp"
// #include <arpa/inet.h>

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

	std::string line, body, response, req;
	std::map<std::string, std::string> dictionary;
	t_files files = getDataFromFiles();

	srand(static_cast<unsigned int>(time(0)));

	int	mode = NORMAL;
	Location	local;
	local.loadConfig();

	while (true)
	{
		std::cout << "\n+++++++ Waiting for a new request ++++++++\n"
					<< std::endl;
		new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (new_socket < 0)
			return (std::cout << "accept failed" << std::endl, -1);

		char buffer[1024];
		int valread = 1;
		// while (valread > 0)
		// {
			valread = read(new_socket, buffer, 1024);
			if (valread > 0)
			{
				std::string str(buffer, valread);
				req.append(str);
			}
		// 	else
		// 		break;
		// 	// std::cout << "====================================================" << std::endl;
		// 	// std::cout << req << std::endl;
		// 	// std::cout << "====================================================" << std::endl;
		// }

		// std::cout << " VALREAD :: " << valread << "\n" << req << std::endl;

		if (valread < 0)
		{
			std::cout << "No bytes are there to read" << std::endl;
			break;
		}

		try
		{
			Request request(req, &mode, &local);
			Response response(new_socket, request, files, request.getError(), &mode);
			std::cout << "------------------ Response sent -------------------\n"
					<< std::endl;
			close(new_socket);
			req = "";
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
		// }
	}
	return 0;
}
