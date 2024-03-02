/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 21:38:04 by iantar           ###   ########.fr       */
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
		// if (ac != 2)
		// 	throw std::runtime_error("Invalid number of arguments!");
		//ConfigFileParser parser(*(av + 1));

		//parser.config_file_parsing();
		VirtualServer	Vser1;
		//
		// std::vector<VirtualServer*>	Ververs = {&Vser1};
		std::vector<VirtualServer*> Ververs;
		Ververs.push_back(&Vser1);
		Server webserv(Ververs);

		webserv.launchServer();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
	(void)ac;
	(void)av;
}
