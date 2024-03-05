/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:57:45 by iantar            #+#    #+#             */
/*   Updated: 2024/03/05 15:49:12 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Location.hpp"

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