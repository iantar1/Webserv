/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 02:15:17 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/22 03:09:35 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class PostResponse
{
	private :
		std::string 						resonseBody, response, strTime, contentType;
		std::string							path, requestBody;
		Request 							request;
		t_files								files;
		std::ofstream						outFile;
		int									contentLenght;
		int									postType;
		int 								socket;

	public :
		PostResponse(int socket, Request &request, t_files files);
		PostResponse(const PostResponse &other);
		PostResponse &operator=(const PostResponse &other);
		~PostResponse(void);

		void	thePostInternalServerError(void);
		void	thePostResponseCreated(void);
		void	thePostMethod(void);

	private :
		class PostResponseException : public std::exception
		{
		private:
			std::string error;

		public:
			PostResponseException(std::string error);
			virtual ~PostResponseException(void) throw();
			const char *what(void) const throw();
		};

};
