/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LaunchVirtualServer.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:17:04 by iantar            #+#    #+#             */
/*   Updated: 2024/02/16 15:40:25 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "headers.hpp"

class LaunchVirtualServer
{
private:
    LaunchVirtualServer(const LaunchVirtualServer&);
    LaunchVirtualServer&    operator=(const LaunchVirtualServer&);
    LaunchVirtualServer();

private:

public:
    static void launch(int port);
    ~LaunchVirtualServer();
};

LaunchVirtualServer::~LaunchVirtualServer()
{
}

void    LaunchVirtualServer::launch(int port)
{
}


