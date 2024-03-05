/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/05 16:35:52 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"
# include "../includes/Request.hpp"

Client::Client(int fdSock, VirtualServer* Vser, t_files* _files) : sockeFd(fdSock), Vserver(Vser),files(_files), request(fdSock, Vser)
{
    Response    response(&request, files);
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
}


void	Client::ServingClient()
{
    
    // allwoed Methode , so you need to constuct Respose with Vserver
    
    DoneServing = true;
}
