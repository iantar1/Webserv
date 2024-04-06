/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/04/06 13:08:28 by nabboune         ###   ########.fr       */
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
	size_t 	chunkContentTotalSizePosted;
	size_t chunkSize;
	int 	mode;
	long long 	contentLenght;
	int 	postType;
	size_t 	ccl;
	size_t 	appendedSize;

	Request *request;
	t_files &files;
	struct stat buffer;
	struct stat buf;

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
	std::string	uploadedFileName;


// cgi inpit/output
	std::string	input_file;
	std::string	output_file;
	// std::map<std::string, std::string>	cgiKeyValue;
// Cgi header
	std::string	cookie;
	std::string contentType_cgi;

	std::string	cgiBody;
	

	bool 	chunkStart;
	bool 	streamStart;
	bool 	outOpened;
	bool 	gotTime;
	bool 	modeChecked;
	bool 	dataCopy;
	bool 	startedTheChunk;
	bool	doneCGI;
	bool	startedPostTime;

	bool	preCGI;
	pid_t pid;
	int status;
	char **env;

	time_t	start;
	time_t	now;

	std::vector<std::string> CgiEnvironment;
	std::map<std::string, std::string>	cgiResponseHeaders;
	// std::map<std::string, std::string>	cgiResponseHeaders;

public:
	Response(Request *request, t_files &files);
	virtual ~Response(void);

	bool	postingDone;

	void errorPage(int errorCode);
	std::string	getPageContent(std::string page);
	void StartResponse();

	void	theGetHeaderResponse(int code, int transferType);
	void	theGetRedirectionRequest(void);
	void	theGetRedirectionIndex(std::string newIndex);
	void	theGetErrorBadRequest(void);
	void	theGetErrorForbidden(void);
	void	theGetErrorNotFound(void);
	void	theGetResponseOk(void);
	void	directoryListing(void);
	void	regularFileGet(void);
	void	theGetMethod(void);
	bool	checkPreGetMethod(void);
	void	servPage(std::string page);

	const std::string	&getResponse() const;

	void	thePostHeaderResponse(int code, int transferType);
	void	thePostInternalServerError(void);
	void	thePostResponseCreatedPage(void);
	void	thePostResponseCreate(void);
	void	thePostMethod(void);
	bool	prePostMethod(void);
	void	PostResponse(void);

	void	cgiResponse(void);
	void	timedOut(void);

	// ******************** Design Post **************

	void posting(void);
	void fillResponse(void);

	// ******************** DelteMethod **************
private:
	void	DeleteMethod();
	int		DeleteDiractory(const std::string &);
	bool	isFile(const std::string &);
	bool	isDiractory(const std::string &);
	int		deleteFile(const std::string &);
	void	theDeleteHeaderResponse(int, int);

	// **************** CGI **************************
private:
	void				cgi_Handler();
	std::string			getExtention() const;
	const std::string	&getCgiPath() const;
	char**				setCgiEnvironment();
	std::string			getScriptName();
	bool				validCGI(const std::string&);
	bool				isCGI();
	std::string			RandomName();
	void				redirectCgiInput();
	void				redirectCgiOutput();
	void				set_args(char**);
	void				extractCgiMetadata();
	void				parseStoreCgiOutHeader(std::string);
	std::string			getCgiFileRoot();
	bool				chechStatus(int status);
	void	print_CGI_env(char **);// debug

// *************** Getters ***************
public:
	const Request*	getRequest() const;

// ******************* setters ********************
	void	setURI();
};
