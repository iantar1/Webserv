/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 04:13:45 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/21 04:16:09 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/definitions.hpp"
#include "../includes/PostResponse.hpp"
#include "../includes/utils.hpp"

PostResponse::PostResponseException::PostResponseException(std::string error) : error(error) {}

const char *PostResponse::PostResponseException::what(void) const throw() { return this->error.c_str(); }

PostResponse::PostResponseException::~PostResponseException(void) throw() {}


PostResponse::PostResponse(int socket, Request &request, t_files files)
{
	this->request = request;
	this->socket = socket;
	this->files = files;

	// thePostMethod();
}

PostResponse::PostResponse(const PostResponse &other) : request(other.request), files(other.files), socket(other.socket) {}

PostResponse &PostResponse::operator=(const PostResponse &other)
{
	this->request = other.request;
	this->socket = other.socket;
	this->files = other.files;
	return (*this);
}

PostResponse::~PostResponse(void) {}

