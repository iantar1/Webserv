/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 03:33:31 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/03 04:26:36 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

Location::Location(void) : autoIndex(false), maxBodySize(0) {}

Location::Location(const Location &other) { *this = other; }

Location&	Location::operator=(const Location &other)
{
	this->cgi = other.cgi;
	return (*this);
}

Location::~Location(void) {}

void	Location::loadConfig(void)
{
	std::string							line, word, key, value;
	std::ifstream						inFile("server.conf");
	std::stringstream					ss;
	std::map<std::string, std::string>	innerMap;

	if (!inFile.is_open())
		throw std::runtime_error("ERROR: unable to open the configuration file!");

	while (std::getline(inFile, line))
	{
		ss.clear();
		ss << line;
		while (ss >> word)
		{
			if (word == "location")
			{
				ss >> word;
				this->location = word;
			}
			else if (word == "allowed_methods")
				while (ss >> word)
					this->allowedMethods.push_back(word);
			else if (word == "max_body_size")
			{
				ss >> word;
				this->maxBodySize = std::atoi(word.c_str());
			}
			else if (word == "upload_post")
			{
				ss >> word;
				this->uploadPost = word;
			}
			else if (word == "upload_path")
			{
				ss >> word;
				this->uploadPath = word;
			}
			else if (word == "error_page")
			{
				ss >> key;
				ss >> value;
				this->errorPage.insert(std::make_pair(std::atoi(key.c_str()), value));
			}
			else if (word == "autoindex")
			{
				ss >> word;
				if (word == "on")
					this->autoIndex = true;
			}
			else if (word == "redirect")
			{
				ss >> word;
				ss >> key;
				ss >> value;
				innerMap.insert(std::make_pair(key, value));
				this->redirect.insert(std::make_pair(std::atoi(word.c_str()), innerMap));
			}
			else if (word == "index")
			{
				ss >> word;
				this->index = word;
			}
			else if (word == "root")
			{
				ss >> word;
				this->root = word;
			}
			else if (word == "cgi")
			{
				ss >> key;
				ss >> value;
				this->cgi.insert(std::make_pair(key, value));
			}
			else
				throw std::runtime_error("ERROR: ba9i matparssa l config file !!!");
		}
	}
	inFile.close();
}

bool												Location::getAutoIndex(void) const { return this->autoIndex; }

size_t												Location::getMaxBodySize(void) const { return this->maxBodySize; }

std::string											Location::getLocation(void) const { return this->location; }

std::string											Location::getUploadPost(void) const { return this->uploadPost; }

std::string											Location::getUploadPath(void) const { return this->uploadPath; }

std::string											Location::getIndex(void) const { return this->index; }

std::string											Location::getRoot(void) const { return this->root; }

std::vector<std::string>							Location::getAllowedMethods(void) const { return this->allowedMethods; }

std::map<int, std::string>							Location::getErrorPage(void) const { return this->errorPage; }

std::map<std::string, std::string>					Location::getCGI(void) const { return this->cgi; }

std::map<int, std::map<std::string, std::string> >	Location::getRedirect(void) const { return this->redirect; }

