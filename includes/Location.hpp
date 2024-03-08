/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:57:42 by iantar            #+#    #+#             */
/*   Updated: 2024/03/08 22:07:38 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"

// GET 0
// POST 1
// DELETE 2
 
class Location
{
private:
    Location(const Location&);
    Location& operator=(const Location&);


public: // you will change this later to private
	std::vector<std::string>	allowedMethods;
	bool		autoIndex;
	long		maxBodySize;
	std::string	uploadPost;
	std::string	uploadPath;
	std::string	index;
	std::string	root;
	std::string	location;

	std::map<int, std::string>			errorPage;
	std::map<std::string, std::string>	cgi;

	std::vector<int>									redirectCode;
	std::vector<std::pair<std::string, std::string> >	redirectLocations;

public:
	Location(std::string);
    Location();
    ~Location();

// ************* Setters *****************

	
// ************* Getters *****************
	const std::string&	getRoot() const;
	const std::string&	getLocation() const;
	long				getMaxBodySize() const;

// ************ Setters *****************
	void	setMaxbodySize(long);
};


// 	allowed_methods POST GET

// 	max_body_size 25000000
// 	upload_post /Uploads/
// 	upload_path /Uploads/
// 	error_page 404 defaultPages/400.htm
// 	autoindex on
// redirect 301 /Media/op.png /Media/op.gif
// redirect 302 /Media/op.mp4 /Media/op.gif
// redirect 301 /Media/op.jpeg /Media/op.gif
// 	index defaultPages/201.htm
// 	root /nfs/homes/nabboune/Desktop/Projects/WebServ/
// 	cgi .py /usr/bin/python3