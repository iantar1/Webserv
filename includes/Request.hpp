/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:49:40 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/03 06:30:04 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Location.hpp"

class Location;

class Request
{
	private :
		std::map<std::string, std::string>	request;
		std::map<std::string, std::string>	method;
		std::string							body;
		std::string							chunkedBodySize;
		Location							*local;
		int									errorCode;

	public :
		Request(void);
		Request(std::string strRequest, int *mode, Location *local);
		Request(const Request &other);
		Request &operator=(const Request &other);
		~Request(void);

		int												getError(void) const;
		bool											checkMethod(void);
		std::string										getBody(void) const;
		std::string										getChunkedBodySize(void) const;
		std::map<std::string, std::string>				getRequest(void) const;
		std::map<std::string, std::string>				getMethod(void) const;
		std::map<std::string, std::string>::iterator	easyfind(std::map<std::string, std::string> &container, std::string x) const;

	private :
		class RequestException : public std::exception
		{
			private :
				std::string	error;

			public :
				RequestException(std::string error);
				virtual ~RequestException(void) throw();
				const char *what(void) const throw();
		};
};
