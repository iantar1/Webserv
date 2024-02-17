/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:49:40 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 03:52:42 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>

class Request
{
	private :
		std::map<std::string, std::string>	request;
		std::map<std::string, std::string>	method;

	public :
		Request(void);
		Request(char *request);
		Request(const Request &other);
		Request &operator=(const Request &other);
		std::map<std::string, std::string>	getRequest(void) const;
		std::map<std::string, std::string>	getMethod(void) const;
		void	checkMethod(void) const;
		~Request(void);

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
