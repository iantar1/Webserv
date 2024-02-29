/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/02/23 10:42:36 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/headers.hpp"

# define PORT 8080
# define CONNECTIN_QUEUE 100

int	main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw std::runtime_error("Invalid number of arguments!");

		// Config File Parsing
		ConfigFileParser parser(*(argv + 1));

		parser.config_file_parsing();

		// Launching the Server
		WebservCore webserv(parser.get_parsing_result());
		webserv.launch_server();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
