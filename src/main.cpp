/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 15:07:38 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/ConfigFileParser.hpp"

# define PORT 8080
# define CONNECTIN_QUEUE 100

int	main(int ac, char **av)
{
	try
	{
		if (ac != 2)
			throw std::runtime_error("Invalid number of arguments!");
		ConfigFileParser parser(*(av + 1));

		parser.config_file_parsing();


		Server webserv(parser.get_parsing_result());
		webserv.launchServer();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
