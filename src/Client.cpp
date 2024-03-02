/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:22:36 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 11:46:44 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Client.hpp"

Client::Client(const VirtualServer* Vser, int sockeFd) : Vserver(Vser), request(sockeFd)
{
}

Client::~Client()
{
}

void	Client::ReadParseReqHeader()
{
    request.ParseRequest();
}