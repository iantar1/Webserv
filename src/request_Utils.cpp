/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_Utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:30:16 by iantar            #+#    #+#             */
/*   Updated: 2024/04/07 02:05:46 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headers.hpp"
#include "../includes/Request.hpp"
#include "../includes/utils.hpp"

static const std::string hex = "0123456789abcdef0123456789ABCDEF";

static int convert(char a, char b)
{
    int res;

    res = (hex.find(a)) % 16;
    res = (res<<4);
    res += (hex.find(b)) % 16;
    return (res);
}

static bool    isHex(char a, char b)
{
    return (hex.find(a) != std::string::npos && hex.find(b) != std::string::npos);
}

void    encoding(std::string& str)
{
    size_t index;

    while ((index = str.find("%")) < str.size() - 2
            && isHex(str[index + 1], str[index + 2]))
    {    
        str = str.substr(0, index)
            + (char)(convert(str[index + 1], str[index + 2]))
                + str.substr(index + 3);
    }
}

