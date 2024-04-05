/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/04/03 05:01:55 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"
# include "../includes/Request.hpp"

Client::Client(int fdSock, const ServerBlock& Vser, t_files _files) :
        sockeFd(fdSock), Vserver(Vser),files(_files), request(fdSock, Vser), doneReading(false) // initialier list
{
    response = new Response(&request, files);
}

Client::~Client()
{
    delete response; 
}

void    Client::PrintRequest() const
{
}

void	Client::ReadParseReqHeader()
{
    request.ReadRequest();
    // std::cout << "*******************FIRST BODY***********************" << request.getBody() << "*************************END**********************" << std::endl;
}


void	Client::ServingClient()
{
    // allwoed Method , so you need to constuct Respose with Vserver
    response->setURI();
    response->StartResponse();
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
    request.setDoneHeaderReading();
}

int		Client::getSocketFd() const { return this->sockeFd; }

Response*	Client::getResponseClass() const { return this->response; }

Request*		Client::getRequest() { return &this->request; }
