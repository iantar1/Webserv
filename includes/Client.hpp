/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/05/28 23:04:00 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/headers.hpp"
#include "../includes/Request.hpp"
#include "../includes/Response.hpp"
#include "../includes/ServerBlock.hpp"

#define POST 1
#define GET 2
#define DELETE 3

// class Request;
// class Response;

class Client
{
private:
	int											sockeFd;
public:
	ServerBlock									Vserver;
private:
	std::map<int, std::vector<ServerBlock> >	VservSimilars;
	t_files										files;
	char										buf[BUF_SIZE];
	int											MethodType;
	time_t										header_time;// an mount of time for the header reading00
	time_t										reading_time;// reading nothing from the client
public:
	bool isServerAssigned;
	Request request;
	Response *response;
	bool doneReading;

	// AMethod*			method;

	// Pravite  Methodes
private:
	Client(const Client &);
	Client &operator=(const Client &);
	Client();

public:
	Client(int, ServerBlock , t_files, std::map<int, std::vector<ServerBlock> >);
	~Client();

	// Request and Respose classes
	// std::map<std::string , AMethod*> respose;

	bool doneServing;

	void ReadParseReqHeader();
	void ServingClient();
	void PrintRequest() const;
	void	timeOutChecking();
	void 	killChild();
	// void assignServer(void);

	// ******* Getters ***************
	bool getDoneServing() const;
	bool getDoneReading() const;
	int getSocketFd() const;
	Response *getResponseClass() const;
	Request *getRequest();
	// ********** Setters *************

	void setDoneServing();
	void setDoneReading();
};
