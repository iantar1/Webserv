/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 16:34:24 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include "../includes/Client.hpp"
#include "../includes/Request.hpp"

Client::Client(int fdSock, ServerBlock Vser, t_files _files, std::map<int, std::vector<ServerBlock> > similars) 
        : sockeFd(fdSock), Vserver(Vser), VservSimilars(similars), files(_files), request(fdSock, Vser, similars), doneReading(false) // initialier list
{
    response = new Response(&request, files);
    header_time = time(NULL);
	isServerAssigned = false;

}

Client::~Client()
{
    delete response;
}

void Client::PrintRequest() const
{
}

void Client::ReadParseReqHeader()
{
    request.ReadRequest();
}

void Client::ServingClient()
{
    // allwoed Method , so you need to constuct Respose with Vserver
    response->setURI();
    response->StartResponse();
}

void Client::killChild()
{
    if (request.getError() && response->childToKill)
    {
        if (kill(response->pid, SIGKILL) == 0)
        {
            wait(NULL);
            sleep(1);
        // wait for the kernenl to kill the child
        }
        else
        {
            std::cerr << "failed to kill child\n";
        }
    }
}

void	Client::timeOutChecking()
{
    killChild();
    if (request.getByteReading())
        reading_time = time(NULL);
    if (request.getDoneHeaderReading())
        header_time = time(NULL);
    if (time(NULL) - header_time > TIMEOUT)
    {
    // ! this should change later , to set a varible in the Cleint Class
        request.setFlagErrorWithoutThrow(REQUEST_TIMEOUT, "request timeOut");
    }
    // if (time(NULL) - reading_time > 1)
        // request.setFlagErrorWithoutThrow(READ_FAILED, "the client flies");
}

// void Client::assignServer(void)
// {
//     if (VservSimilars.empty() || VservSimilars.find(Vserver.getSid()) == VservSimilars.end())
//         return;
//     if (request.getHost().empty() || Vserver.findHostName(request.getHost()))
//         return;
//     std::vector<ServerBlock>::iterator it = VservSimilars[Vserver.getSid()].begin();
//     for (; it != VservSimilars[Vserver.getSid()].end(); it++)
//     {
//         if (it->findHostName(request.getHost()))
//         {
//             it->setFdSocket(Vserver.getFdSocket());
//             Vserver = *it;
//             request.Vserver = *it;
//         }
//     }
// }

// ************** Gettter *********************

bool Client::getDoneServing() const
{
    return (request.getDoneServing());
}

void Client::setDoneServing()
{
    this->doneServing = true;
}

bool Client::getDoneReading() const
{
    return (doneReading);
}

void Client::setDoneReading()
{
    doneReading = true;
    request.setDoneHeaderReading();
}

int Client::getSocketFd() const { return this->sockeFd; }

Response *Client::getResponseClass() const { return this->response; }

Request *Client::getRequest() { return &this->request; }
