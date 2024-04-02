/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_iantar.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 00:43:38 by iantar            #+#    #+#             */
/*   Updated: 2024/04/02 00:51:13 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"

// check status code
// map error pages map<int, std::string>

std::map<int, std::string> error_page;

void Response::theGetErrorNotFound(int status)
{
	
	this->contentType = "text/html";
	this->body = errpr_page(status) + "\r\n\r\n";
	theGetHeaderResponse(NOT_FOUND, CONTENT_LENGHT);
	this->response += this->body;
	this->request->setDoneServing();
}