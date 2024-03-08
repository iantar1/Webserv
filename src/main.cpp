/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 09:44:50 by iantar            #+#    #+#             */
/*   Updated: 2024/03/08 15:29:45 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# include "../includes/VirtualServer.hpp"

# include "../includes/ConfigFileParser.hpp"
# include <iostream>
# define PORT 8080
# define CONNECTIN_QUEUE 100

# include "../includes/Server.hpp"

Location*	getLocation1(std::string location)
{
	Location* loc = new Location(location);

	loc->allowedMethods.push_back("POST");
	loc->allowedMethods.push_back("GET");
	loc->allowedMethods.push_back("DELETE");
	
	loc->uploadPath = "/nfs/homes/iantar/Desktop/Webserv";
	loc->uploadPost = "/nfs/homes/iantar/Desktop/Webserv/";
	loc->index = "page.html";
	loc->root = "/nfs/homes/iantar/Desktop/Webserv/";
	loc->location =  "/";
	
	loc->errorPage[404] = "";
	loc->cgi[".py"] = "/usr/bin/python3";

	loc->redirectCode.push_back(301);
	loc->redirectLocations.push_back(std::make_pair("/Media/op.png", "/Media/op.gif"));
	
	loc->redirectCode.push_back(302);
	loc->redirectLocations.push_back(std::make_pair("/Media/op.mp4", "/Media/op.gif"));
	
	return (loc);
}

// iantarPath: 

Location*	getLocation2(std::string location)
{
	Location* loc = new Location(location);

	loc->allowedMethods.push_back("POST");
	loc->allowedMethods.push_back("GET");
	loc->allowedMethods.push_back("DELETE");
	
	loc->uploadPath = "/nfs/homes/iantar/Desktop/Webserv";
	loc->uploadPost = "/nfs/homes/iantar/Desktop/Webserv";
	loc->index = "page.html";
	loc->root = "/nfs/homes/iantar/Desktop/Webserv";
	loc->location =  "/";
	
	loc->errorPage[404] = "";
	loc->cgi[".py"] = "/usr/bin/python3";

	loc->redirectCode.push_back(301);
	loc->redirectLocations.push_back(std::make_pair("/Media/op.png", "/Media/op.gif"));
	
	loc->redirectCode.push_back(302);
	loc->redirectLocations.push_back(std::make_pair("/Media/op.mp4", "/Media/op.gif"));
	
	return (loc);
}

void fillVector(std::vector<VirtualServer*>& Vser)
{
	VirtualServer	*Vser2 = new VirtualServer();
	Vser2->SetLocation(getLocation2("/"), "/");
	Vser2->Port = "8080";
	Vser2->HostAddress = "localhost";
	Vser.push_back(Vser2);

	// VirtualServer	*Vser1 = new VirtualServer();
	// Vser1->SetLocation(getLocation1("/"), "/");
	// Vser1->Port = "8081";
	// Vser1->HostAddress = "localhost";
	// Vser.push_back(Vser1);
}

int	main(int ac, char **av)
{
	
	try
	{
		std::vector<VirtualServer*> Vserv;

		fillVector(Vserv);
		Server webserv(Vserv);

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
