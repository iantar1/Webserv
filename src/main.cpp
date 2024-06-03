/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 19:07:29 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ********************************************************************************* */
/*                                                                                   */
/*                                  	     :::   ::::::::   ::::::::  :::::::::::  */
/*  main.cpp                               :+:+:         :+:        :+: :+:     :+:  */
/*                                           +:+         +:+        +:+        +:+   */
/*  By: iantar@student.1337.ma               +#+      +#++:      +#++:        +#+    */
/*                                           +#+         +#+        +#+      +#+     */
/* Created: 2024/02/08 09:44:50 by iantar    #+#         #+# #+#    #+#     #+#      */
/* Updated: 2024/03/25 03:10:50 by iantar  ####### ########   ########     ###.ma    */
/*           	                                                                     */
/* ********************************************************************************* */

#include "../includes/headers.hpp"
#include "../includes/macros.hpp"
#include "../includes/Server.hpp"
#include "../includes/Config.hpp"

#define DEFAULT_CONFIG "config/default.conf"

void printConfigParsing(const std::vector<ServerBlock> &servers)
{
	for (std::size_t i = 0; i < servers.size(); i++)
	{
		std::cout << "Server: " << i << std::endl;
		std::cout << servers[i] << std::endl;
		std::cout << "-----------------------------------\n"
				  << std::endl;
	}
}

int main(int ac, char **av)
{
	std::string cnfPath[2] = {"config/default.conf", av[ac - 1]};
	std::vector<ServerBlock> servers;

	if (ac > 2)
	{
		std::cerr << "Invalid arrg\n";
		return (EXIT_FAILURE);
	}
	try
	{
		Config config(cnfPath[ac - 1]);
		config.parse();
		servers = config.servers;
		// printConfigParsing(servers);
		Server webserv(config.servers, config.similars);
		webserv.ServerCore();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
