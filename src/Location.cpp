/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:57:45 by iantar            #+#    #+#             */
/*   Updated: 2024/03/08 17:01:43 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Location.hpp"

Location::Location(std::string locStr) : location(locStr)
{
}

Location::Location()
{
}

Location::~Location()
{
}

// ************** getters ******************

const std::string& Location::getRoot() const
{
    return (root);
}

const std::string& Location::getLocation() const
{
    return (location);
}

long  Location::getMaxBodySize() const
{
    return (this->maxBodySize);
}

// *********** Setters ************

void    Location::setMaxbodySize(long _maxBodySize)
{
    this->maxBodySize = _maxBodySize;
}

