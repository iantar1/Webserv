/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/03/23 23:21:23 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/headers.hpp"
// #include "../includes/macros.hpp"
// // #include "../includes/VirtualServer.hpp"

// #include <iostream>
// #define PORT 8080
// #define CONNECTIN_QUEUE 100

// #include "../includes/Server.hpp"

// Location *getLocation1(std::string location)
// {
// 	Location *loc = new Location(location);

// 	loc->allowedMethods.push_back("POST");
// 	loc->allowedMethods.push_back("GET");
// 	loc->allowedMethods.push_back("DELETE");

// 	loc->uploadPath = "/nfs/homes/iantar/Desktop/Webserv";
// 	loc->uploadPost = "/nfs/homes/iantar/Desktop/Webserv";
// 	loc->index = "page.html";
// 	loc->root = "/nfs/homes/iantar/Desktop/Webserv"; // ! it must came without / at the end
// 	loc->location = location;

// 	loc->errorPage[404] = "";
// 	loc->cgi[".py"] = "/usr/bin/python3";
// 	loc->cgi[".sh"] = "/usr/bin/sh";
// 	loc->cgi[".php"] = "/usr/bin/php-cgi";

// 	loc->redirectCode.push_back(301);
// 	loc->redirectLocations.push_back(std::make_pair("/Media/op.png", "/Media/op.gif"));

// 	loc->redirectCode.push_back(302);
// 	loc->redirectLocations.push_back(std::make_pair("/Media/op.mp4", "/Media/op.gif"));
// 	loc->setMaxbodySize(90000000);

// 	return (loc);
// }

// // /nfs/homes/iantar/Desktop/Webserv
// // /nfs/homes/iantar/Desktop/Webserv

// Location *getLocation2(std::string location)
// {
// 	Location *loc = new Location(location);

// 	loc->allowedMethods.push_back("POST");
// 	loc->allowedMethods.push_back("GET");
// 	loc->allowedMethods.push_back("DELETE");

// 	loc->uploadPath = "/nfs/homes/iantar/Desktop/Webserv";
// 	loc->uploadPost = "/nfs/homes/iantar/Desktop/Webserv";
// 	loc->index = "page.html";
// 	loc->root = "/nfs/homes/iantar/Desktop/Webserv";
// 	loc->location = location;

// 	loc->errorPage[404] = "";
// 	loc->cgi[".py"] = "/usr/bin/python3";
// 	loc->cgi[".sh"] = "/usr/bin/sh";
// 	loc->cgi[".php"] = "/usr/bin/php-cgi";

// 	loc->redirectCode.push_back(301);
// 	loc->redirectLocations.push_back(std::make_pair("/Media/op.png", "/Media/op.gif"));

// 	loc->redirectCode.push_back(302);
// 	loc->redirectLocations.push_back(std::make_pair("/Media/op.mp4", "/Media/op.gif"));
// 	loc->setMaxbodySize(90000000);

// 	return (loc);
// }

// void fillVector(std::vector<VirtualServer *> &Vser)
// {
// 	VirtualServer *Vser2 = new VirtualServer();
// 	Vser2->SetLocation(getLocation2("/"), "/");
// 	Vser2->Port = "8081";
// 	Vser2->HostAddress = "0.0.0.0";
// 	Vser.push_back(Vser2);

// 	VirtualServer *Vser1 = new VirtualServer();
// 	Vser1->SetLocation(getLocation1("/"), "/");
// 	Vser1->Port = "8080";
// 	Vser1->HostAddress = "0.0.0.0";
// 	Vser.push_back(Vser1);
// }


#include "../includes/Config.hpp"


int main(int ac, char **av)
{
	try
	{
		// std::vector<VirtualServer *> Vserv;
		Config config("config/config.conf");
		config.parse();
		std::vector<ServerBlock> servers = config.servers;
		for (std::size_t i = 0; i < servers.size(); i++)
		{
			std::cout << "Server: " << i << std::endl;
			std::cout << servers[i] << std::endl;
			std::cout << "-----------------------------------\n" << std::endl;
		}
		// fillVector(Vserv); // Ibrahim
		// Server webserv(Vserv);

		// webserv.ServerCore();
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
