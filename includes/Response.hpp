/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/09 17:24:16 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "Webserv.hpp"
# include "Request.hpp"
#include "utils.hpp"



class Response
{
private:
	Response(const Response&);
	Response &operator=(const Response&);

	private :
		int 	socket;
		std::string	responseBody, response, strTime;
		t_files	files;
		Request*	request;

	public :
		Response(Request* request, t_files files);
		virtual ~Response(void);

		void	errorPage(int errorCode);
		void	thePostMethod(void);
		void	theDeleteMethod(void);
		void	StartResponse();
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
