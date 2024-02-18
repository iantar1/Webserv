/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 23:37:51 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"

class Response
{
private:
	std::map<std::string, std::string> header, mime;
	std::string body, response;
	Request request;
	int socket;
	// Response(void);

public:
	Response(int socket, Request &request, std::map<std::string, std::string> mime);
	Response(const Response &other);
	Response &operator=(const Response &other);
	std::map<std::string, std::string>::iterator easyfind(std::map<std::string, std::string> &container, std::string x) const;
	void theGetMethod(void);
	void thePostMethod(void);
	void theDeleteMethod(void);
	~Response(void);

private:
	class ResponseException : public std::exception
	{
	private:
		std::string error;

	public:
		ResponseException(std::string error);
		virtual ~ResponseException(void) throw();
		const char *what(void) const throw();
	};
};
