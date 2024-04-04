/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:09:09 by nabboune          #+#    #+#             */
/*   Updated: 2024/04/02 01:59:38 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/macros.hpp"
#include "../includes/Response.hpp"
#include "../includes/utils.hpp"

Response::Response(Request *request, t_files &files) : request(request), files(files)
{
	this->socket = request->getFdSocket();
	// post method
	this->postedSize = 0;
	this->contentLenght = 0;
	this->chunkSize = 0;
	this->appendedSize = 0;
	this->transferMode = request->getTransferMode();
}


Response::~Response()
{
	if (this->inFile.is_open())
		this->inFile.close();
	if (this->postOutFile.is_open())
		this->postOutFile.close();
}

std::string Response::getPageContent(std::string page)
{
	std::string pgNbStr = (split(page, '/').back());
	std::istringstream pg(pgNbStr);
	int pgNb;
	pg >> pgNb;
	if (request->Vserver.getErrorPages().find(pgNb) != request->Vserver.getErrorPages().end())
	{
		page = request->Vserver.getErrorPages().find(pgNb)->second;
	}
	std::ifstream inFile(page.c_str());
	std::string line, pageContent = "";

	if (inFile.is_open())
	{
		while (std::getline(inFile, line))
		{
			pageContent += line;
			pageContent += "\n";
		}
	}
	inFile.close();
	return pageContent;
}

// *************** Seters *********

void Response::setURI()
{
	this->uri = this->request->getURI();
}
