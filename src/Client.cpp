/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/02 20:37:48 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"
# include "../nabboneWork/includes/GetResponse.hpp"

Client::Client(const VirtualServer* Vser, int fd) : sockeFd(fd), Vserver(Vser), DoneServing(false), DoneHeaderReading(false) 
{
}

Client::~Client()
{
}

void	Client::ReadParseReqHeader()
{
    // request.ParseRequest();
    DoneHeaderReading = true;
}


void	Client::ServingClient()
{
    t_files file;
    file = getDataFromFiles();
    // allwoed Methode , so you need to constuct Respose with Vserver
    
    GetResponse GetResponse1(sockeFd, request, file);
    DoneServing = true;
}