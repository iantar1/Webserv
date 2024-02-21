/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:39:14 by nabboune          #+#    #+#             */
/*   Updated: 2024/02/20 20:13:57 by nabboune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <fstream>
#include <sstream>
#include <ctime>
#include <map>
#include "definitions.hpp"

template <typename T>
std::string ToString(T num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}
