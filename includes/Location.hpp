/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 00:57:50 by nabboune          #+#    #+#             */
/*   Updated: 2024/03/02 07:44:25 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "utils.hpp"

class Location
{
	private :
		bool												autoIndex;
		size_t												maxBodySize;
		std::string											uploadPost, uploadPath, index, root, location;
		std::vector<std::string>							allowedMethods;
		std::map<int, std::string>							errorPage;
		std::map<std::string, std::string>					cgi;
		std::map<int, std::map<std::string, std::string> >	redirect;

	public :
		Location(void);
		Location(const Location &other);
		Location	&operator=(const Location &other);
		~Location(void);

		void												loadConfig(void);
		bool												getAutoIndex(void) const;
		size_t												getMaxBodySize(void) const;
		std::string											getLocation(void) const;
		std::string											getUploadPost(void) const;
		std::string											getUploadPath(void) const;
		std::string											getIndex(void) const;
		std::string											getRoot(void) const;
		std::vector<std::string>							getAllowedMethods(void) const;
		std::map<int, std::string>							getErrorPage(void) const;
		std::map<std::string, std::string>					getCGI(void) const;
		std::map<int, std::map<std::string, std::string> >	getRedirect(void) const;
};
