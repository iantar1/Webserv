/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/28 23:29:07 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class Response
{
	private :
		int 	socket;
		std::string	responseBody, response, strTime;
		t_files	files;
		Request	request;

	public :
		Response(int socket, Request &request, t_files files, int error, int *mode);
		Response(const Response &other);
		Response &operator=(const Response &other);
		virtual ~Response(void);

		void	errorPage(int errorCode);
		void	thePostMethod(void);
		void	theDeleteMethod(void);

	private :
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
