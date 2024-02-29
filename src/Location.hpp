/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:57:42 by iantar            #+#    #+#             */
/*   Updated: 2024/02/27 11:00:51 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"

class Location
{
private:

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
