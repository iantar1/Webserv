/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:06:49 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 04:07:31 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

std::string dec_to_hex(int decimal);
std::map<std::string, std::string>	mimeTypes(void);

std::map<std::string, std::string>::iterator	easyfind(std::map<std::string, std::string> &container, std::string x)
{
	std::map<std::string, std::string>::iterator	it = container.begin();
	while (it != container.end())
	{
		if (it->first == x)
			return it;
		it++;
	}
	return it;
}
