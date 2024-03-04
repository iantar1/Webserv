/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/04 16:10:50 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"
# include "../includes/Request.hpp"

Client::Client(const VirtualServer* Vser, int fd) : sockeFd(fd), Vserver(Vser)
{
}

Client::~Client()
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
