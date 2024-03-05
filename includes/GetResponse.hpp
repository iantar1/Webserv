/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 03:47:10 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/05 13:33:20 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class GetResponse
{

private:
	GetResponse(const GetResponse &other);
	GetResponse &operator=(const GetResponse &other);

	private :
		std::string 						header, body, response, strTime, contentType, path, oldPath, redirection;
		Request 							*request;
		t_files								*files;
		std::ifstream						inFile;
		int 								socket;

	public :
		GetResponse(int socket, Request* request, t_files* files);
		~GetResponse(void);

		void	theGetHeaderResponse(int code, int transferType);
		void	theGetRedirectionRequest(void);
		void	theGetErrorBadRequest(void);
		void	theGetErrorForbidden(void);
		void	theGetErrorNotFound(void);
		void	theGetResponseOk(void);
		void	directoryListing(void);
		void	regularFileGet(void);
		void	theGetMethod(void);

	private :
		class GetResponseException : public std::exception
		{
		private:
			std::string error;

		public:
			GetResponseException(std::string error);
			virtual ~GetResponseException(void) throw();
			const char *what(void) const throw();
		};

};
