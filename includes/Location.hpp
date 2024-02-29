/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:57:42 by iantar            #+#    #+#             */
/*   Updated: 2024/02/29 11:10:58 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"

class Location
{
private:

    // root:
	// allowed_methods:
	// redirect:
	// index:
	// directory_listing:
	// upload_post1:
	// cgi:

public:
    Location();
    ~Location();
    Location(const Location&);
    Location& operator=(const Location&);
};

Location::Location()
{
}

Location::~Location()
{
}
