/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/23 02:27:21 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class Response
{
private:
	Response(const Response &);
	Response &operator=(const Response &);

private:
	int 	socket;
	int 	contentTotalSizePosted;
	int 	chunkContentTotalSizePosted;
	size_t chunkSize;
	int 	mode;
	int 	contentLenght;
	int 	postType;
	int 	ccl;
	size_t 	appendedSize;

	Request *request;
	t_files &files;

	std::ifstream inFile;
	std::ofstream outFile;

	std::string responseBody;
	std::string header;
	std::string body;
	std::string response;
	std::string strTime;
	std::string strTime2;
	std::string contentType;
	std::string path;
	std::string oldPath;
	std::string redirection;
	std::string requestBody;
	std::string appendedRequest;
	std::string	uri;
// cgi inpit/output
	std::string	input_file;
	std::string	output_file;

	bool 	chunkStart;
	bool 	streamStart;
	bool 	outOpened;
	bool 	gotTime;
	bool 	modeChecked;
	bool 	dataCopy;
	bool 	startedTheChunk;
	bool	doneCGI;

	std::vector<std::string> CgiEnvironment;

public:
	Response(Request *request, t_files &files);
	virtual ~Response(void);

	void errorPage(int errorCode);
	void theDeleteMethod(void);
	void StartResponse();

	void theGetHeaderResponse(int code, int transferType);
	void theGetRedirectionRequest(void);
	void theGetErrorBadRequest(void);
	void theGetErrorForbidden(void);
	void theGetErrorNotFound(void);
	void theGetResponseOk(void);
	void directoryListing(void);
	void regularFileGet(void);
	void theGetMethod(void);
	void Delete();

	const std::string &getResponse() const;
	void PostResponse();

	void thePostHeaderResponse(int code, int transferType);
	void thePostResponseCreate(void);
	void thePostInternalServerError(void);
	void thePostResponseCreatedPage(void);
	void thePostMethod(void);

	// ******************** DelteMethod **************
private:
	void	DeleteMethod();
	int		DeleteDiractory(const std::string &);
	bool	isFile(const std::string &);
	bool	isDiractory(const std::string &);
	bool	deleteFile(const std::string &);
	void	theDeleteHeaderResponse(int, int);

	// **************** CGI **************************
private:
	void				cgi_Handler();
	std::string			getExtention() const;
	const std::string	&getCgiPath() const;
	void				setCgiEnvironment(char**);
	std::string			getScriptName();
	bool				validCGI(const std::string&);
	bool				isCGI();
	std::string			RandomName();
	void				redirectCgiInput();
	void				redirectCgiOutput();
	void				set_args(char**);
// *** debug
	void	print_CGI_env(char **);

// *************** Getters ***************
public:
	const Request*	getRequest() const;

// ******************* setters ********************
	void	setURI();
};
