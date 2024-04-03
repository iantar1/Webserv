/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:28:09 by iantar            #+#    #+#             */
/*   Updated: 2024/03/25 04:27:02 by iantar           ###   ########.fr       */
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
	int sockeFd;
	const ServerBlock &Vserver;
	t_files files;
	char buf[BUF_SIZE];
	int MethodType;
public:
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
	Client(int, const ServerBlock& , t_files);
	~Client();

	// Request and Respose classes
	// std::map<std::string , AMethod*> respose;

	bool doneServing;

	void ReadParseReqHeader();
	void ServingClient();
	void PrintRequest() const;

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
