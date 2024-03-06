/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/06 16:06:39 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"
# include "../includes/Request.hpp"

Client::Client(int fdSock, VirtualServer* Vser, t_files* _files) : sockeFd(fdSock), Vserver(Vser),files(_files), request(fdSock, Vser)
{
    response = new Response(&request, files);
    std::cout << RED << "Cleint Constructred\n" << RESET;
}

Client::~Client()
{
}

void    Client::PrintRequest() const
{
}

void	Client::ReadParseReqHeader()
{
    request.ParseRequest();

    DoneHeaderReading = true;
    // std::cout << "*************** HEADER **************\n" << buf << "\n";
}


void	Client::ServingClient()
{
    
    // allwoed Methode , so you need to constuct Respose with Vserver
    response->StartResponse();
    std::cout << "start Response\n";
    DoneServing = true;
}
