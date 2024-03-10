/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 02:15:17 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/10 15:35:04 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class PostResponse
{
private:
	PostResponse(const PostResponse &other);
	PostResponse &operator=(const PostResponse &other);

	private :
		std::string 						body, response, strTime, contentType;
		std::string							path, requestBody;
		Request 							*request;
		t_files								files;
		std::ofstream						outFile;
		int									contentLenght;
		int									postType;
		int 								socket;
		int									contentTotalSizePosted;
		int									mode;

	public :
		PostResponse(int socket, Request *request, t_files files);
		~PostResponse(void);

		void	thePostHeaderResponse(int code, int transferType);
		void	thePostResponseCreate(int *mode);
		void	thePostInternalServerError(void);
		void	thePostResponseCreatedPage(void);
		void	thePostMethod(int *mode);

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
