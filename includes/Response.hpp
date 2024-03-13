/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/13 23:26:17 by iantar           ###   ########.fr       */
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
		int					chunkContentTotalSizePosted;
		int					chunkSize;
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
		void					DeleteMethod(const std::string&);

		void					thePostHeaderResponse(int code, int transferType);
		void					thePostResponseCreate(void);
		void					thePostInternalServerError(void);
		void					thePostResponseCreatedPage(void);
		void					thePostMethod(void);
	
// ******************** DelteMethod **************
	private:
		bool	isFile(const std::string&);
		bool	isDiractory(const std::string&);
		bool	deleteFile(const std::string&);
		int		deleteDiractory(const std::string&);
};
