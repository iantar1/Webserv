/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:14:10 by iantar            #+#    #+#             */
/*   Updated: 2024/03/02 15:10:36 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

class Response
{
private: // Attributes
    int fdSocket;

    std::map<std::string, AMethod&> methods;
    const Request&         &request;
    //const VirtualServer   *Vserver;
    //const Location        *location;

private:
    Response(const Response&);
    Response& operator=(const Response&);


    

public:
    Response();
    Response(int, const Request&);
    ~Response();
};

Response()::Response(int fd, const Request& _req) : fdSocket(fd), request(_req)
{
}