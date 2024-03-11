/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/11 17:22:28 by nabboune         ###   ########.fr       */
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
		int 				socket;
		int					contentTotalSizePosted;
		int					mode;
		int					contentLenght;
		int					postType;
		int					ccl;

		Request				*request;
		t_files				&files;

		std::ifstream		inFile;
		std::ofstream		outFile;

		std::string			responseBody;
		std::string			header;
		std::string			body;
		std::string			response;
		std::string			strTime;
		std::string			contentType;
		std::string			path;
		std::string			oldPath;
		std::string			redirection;
		std::string			requestBody;

		bool				streamStart;
		bool				outOpened;
		bool				gotTime;
		bool				modeChecked;
		bool				dataCopy;
		bool				startedTheChunk;

	public :
		Response(Request* request, t_files &files);
		virtual ~Response(void);

		void					errorPage(int errorCode);
		// void					thePostMethod(void);
		void					theDeleteMethod(void);
		void					StartResponse();


		void					theGetHeaderResponse(int code, int transferType);
		void					theGetRedirectionRequest(void);
		void					theGetErrorBadRequest(void);
		void					theGetErrorForbidden(void);
		void					theGetErrorNotFound(void);
		void					theGetResponseOk(void);
		void					directoryListing(void);
		void					regularFileGet(void);
		void					theGetMethod(void);
		void					Delete();


		const std::string		&getResponse() const;


		void					PostResponse();

		void					thePostHeaderResponse(int code, int transferType);
		void					thePostResponseCreate(void);
		void					thePostInternalServerError(void);
		void					thePostResponseCreatedPage(void);
		void					thePostMethod(void);

};
