/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/09 22:50:26 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"
# include "../includes/Request.hpp"

Client::Client(int fdSock, VirtualServer* Vser, t_files _files) :
        sockeFd(fdSock), Vserver(Vser),files(_files), request(fdSock, Vser), doneReading(false) // initialier list
{
    response = new Response(&request, files);
    // std::cout << RED << "Cleint Constructred\n" << RESET;
}

Client::~Client()
{
    std::cout << RED << "Cleint Dessss/...\n" << RESET;
    delete response; 
}

void    Client::PrintRequest() const
{
}

void	Client::ReadParseReqHeader()
{
    request.ParseRequest();
}


void	Client::ServingClient()
{
    // allwoed Methode , so you need to constuct Respose with Vserver
    response->StartResponse();
    // std::cout << "start Response\n";
}

// ************** Gettter *********************

bool    Client::getDoneServing() const
{
    return (request.getDoneServing());
}

void    Client::setDoneServing()
{
    this->doneServing = true;
}

bool	Client::getDoneReading() const
{
	return (doneReading);
}

void    Client::setDoneReading()
{
    doneReading = true;
    request.setDoneReading();
}
