/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:14:10 by iantar            #+#    #+#             */
/*   Updated: 2024/03/04 16:08:48 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

// # include "../nabboneWork/includes/Request.hpp"

// # define POST 
// # define GET
// # define DELETE

class Response
{
private: // Attributes
    int fdSocket;
    int MethodType;

    //std::map<std::string, AMethod&> methods;
    Request*        request;
    //const VirtualServer   *Vserver;
    //const Location        *location;

private:
    Response();
    Response(const Response&);
    Response& operator=(const Response&);

// MEthode (private)
    // void GET();
    // void POST();
    // void DELETE();
    

public:
    Response(const Request&); // Request will have all infos you need 
    // you will use getters to extract them
    ~Response();

// main function , find a good name 
    void serverCleint();
};

// Response::Response(int fd, const Request& _req) : fdSocket(fd), request(_req)
// {
// }