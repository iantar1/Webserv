/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/21 04:08:51 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class Response
{
	private :
		std::string 						body, response, strTime, contentType;
		Request 							request;
		t_files								files;
		std::ifstream						inFile;
		std::string							path;
		int 								socket;

	public :
		Response(int socket, Request &request, t_files files);
		Response(const Response &other);
		Response &operator=(const Response &other);
		virtual ~Response(void);

		void thePostMethod(void);
		void theDeleteMethod(void);


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
