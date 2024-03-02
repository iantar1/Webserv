/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 10:21:37 by iantar            #+#    #+#             */
/*   Updated: 2024/03/02 18:17:28 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/headers.hpp"
# include "../includes/Response.hpp"

Response::Response() 
{
}

Response::Response(int fd, Request* res) : fdSocket(fd), request(res)
{

}

Response::~Response()
{
}