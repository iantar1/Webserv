/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:06:49 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/17 17:39:29 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

std::string	dec_to_hex(int decimal);
std::map<std::string, std::string>	mimeTypes(void);
size_t	find_last_of(const std::string& str, char ch);
std::string	getFileExtension(const std::string& fileName);
