/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:30:58 by iantar            #+#    #+#             */
/*   Updated: 2024/03/01 11:48:39 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"

# define BAD_REQ 400 
# define NOT_IMPLEMENTED 501
# define REQ_URI_TOO_LONG 414
# define REQ_ENTITY_TOO_LONG 413  


class Request
{
private:
	char								buf[BUF_SIZE];
    int                                 SocketFd;
    int                                 bytesRead;
    int                                 errorFlag;
    std::string                         RequestHeader;
    static std::string					Methods[3];
    std::map<std::string, std::string>	Header;
	int									MethodType;
	std::vector<std::string>			RequestLine;
	bool								reading_done;

private:
// useless Constructor , copy constructor and assingment operator
    Request(const Request&);
    Request& operator=(const Request&);

public:
    Request(int);
    Request();
    ~Request();

    void    readHeader(const std::string&, size_t);
	void	storeHeader(const std::string&);
	void	storeRequestLine(const std::string&);
	void	storeData(const std::string&, size_t);

    void    ParseRequest();

};
