/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 00:56:12 by nabboune          #+#    #+#             */
/*   Updated: 2024/06/02 17:50:38 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// #include "Webserv.hpp"
#include "Request.hpp"
#include "utils.hpp"

class Response
{
private:
	Response(const Response&);
	Response& operator=(const Response&);
public:
	time_t	startCgiTime;
	bool	childToKill;
	pid_t	pid;

private:
	int 	socket;
	int 	oldContentTotalSizePosted;
	int 	contentTotalSizePosted;
	size_t 	chunkContentTotalSizePosted;
	size_t chunkSize;
	int 	mode;
	long long 	contentLenght;
	int 	postType;
	size_t 	ccl;
	size_t 	appendedSize;

	Request* request;
	t_files& files;
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
	bool		oneTime;// ! to fix a problem in cgi

	// cgi inpit/output
	std::string	input_cgi;
	std::string	output_cgi;
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
	bool	startedPostTime;
	bool	cgiContentLength;
	bool	secondLoop;

	bool	preCGI;
	int status;
	char** env;

	time_t	start;
	time_t	now;

	// ! we must add a vriable called responseDone to check it insted of the one in request
	int	responseDone;


	std::vector<std::string> CgiEnvironment;
	std::map<std::string, std::string>	cgiResponseHeaders;
	// std::map<std::string, std::string>	cgiResponseHeaders;

public:
	std::string	cgiIndexPath;
	bool	constructedCgiHeader;
	bool	doneCGI;
	Response(Request* request, t_files& files);
	~Response(void);

	bool	postingDone;

	void errorPage(int errorCode);
	std::string	getPageContent(std::string page);
	std::string generatePage(int statusCode);
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
	bool	prePre(void); // ingeniosity of Ibrahim and Nouh

	const std::string& getResponse() const;

	void	thePostHeaderResponse(int code, int transferType);
	void	thePostInternalServerError(void);
	void	thePostResponseCreatedPage(void);
	void	thePostResponseCreate(void);
	void	thePostMethod(void);
	bool	prePostMethod(void);
	void	PostResponse(void);

	void	cgiResponse(void);
	bool	timedOut(void);

	// ******************** Design Post **************

	void posting(void);
	void fillResponse(void);

	// ******************** DelteMethod **************
public:
	void	DeleteMethod();
private:
	int		DeleteDiractory(const std::string&);
	bool	isFile(const std::string&);
	bool	isDiractory(const std::string&);
	int		deleteFile(const std::string&);
	void	theDeleteHeaderResponse(int, int);

	// **************** CGI **************************
private:
	std::string			getExtention() const;
	std::string			getExtention(const std::string&) const;
	void				set_args(char**, const std::string&);
	const std::string& getCgiPath() const;
	char** setCgiEnvironment();
	char** setCgiEnvironment(const std::string&);
	std::string			getScriptName();
	// std::string			getScriptName(const std::string&);
	bool				validCGI(const std::string&);
	std::string			RandomName();
	void				redirectCgiInput();
	void				redirectCgiOutput();
	void				set_args(char**);
	void				extractCgiMetadata();
	void				parseStoreCgiOutHeader(std::string);
	std::string			getCgiFileRoot();
	std::string			getCgiFileRoot(const std::string&);
	bool				chechStatus(int status);
	void	print_CGI_env(char**);// debug
	bool				setCgiNewPath();

	// *************** Getters ***************
public:
	bool				indexExist;
	bool				cgi_Handler();
	bool				cgi_Handler(const std::string&);
	bool				indexCgiCheck(void);
	bool				isCGI();
	bool				isCGI(const std::string&);
	const Request* getRequest() const;
	bool 				isThisPathExist(std::string, std::string);//!hada llah
	// ******************* setters ********************
	void	setURI();
};
