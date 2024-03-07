/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:30:58 by iantar            #+#    #+#             */
/*   Updated: 2024/03/07 14:51:45 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "headers.hpp"
# include "macros.hpp"
# include "VirtualServer.hpp"

# define BAD_REQ 400 
# define NOT_IMPLEMENTED 501
# define REQ_URI_TOO_LONG 414
# define REQ_ENTITY_TOO_LONG 413  


typedef	std::map<std::string, Location *>::const_iterator	mapIterType;

class Request
{
private:
// useless Constructor , copy constructor and assingment operator
    Request();
    Request(const Request&);
    Request& operator=(const Request&);

private:
	char								buf[BUF_SIZE];
    int                                 SocketFd;
    int                                 bytesRead;
    int                                 ErrorFlag; // when you face an error , set a Macro to it 
    std::string                         RequestHeader;
    std::map<std::string, std::string>	Header;
	int									MethodType;
	std::vector<std::string>			RequestLine;// this line: GET /hello.htm HTTP/1.1
	bool								reading_done;
    int                                 TransferMode;  // Chuncked() or normal(content length)
    int                                 ErrorCode;
    std::string                         chunkedBodySize;
    std::string                         body;
    VirtualServer                       *Vserver;
    std::string                         newPath;
    std::string                         oldPath;
    std::string                         HeaderReq;

// static attrebuites

    static std::string					Methods[3];
	static std::string					validChars;

// ***************** Private Methodes **************
private:
    void	SetNewPath();

    void	URI_Checking(const std::string&);
    bool	URI_ValidLength(const std::string&) const;
    bool    URI_ValidChar(const std::string&) const;
    bool	URI_ValidLocation(const std::string&) const;

public:
// ************ Constructor destructor
    Request(int,  VirtualServer *);
    ~Request();

// ******** Public Methods
    void    readHeader(const std::string&, size_t);
	void	storeHeader(const std::string&);
	void	storeRequestLine(const std::string&);
	void	storeData(const std::string&);

    void    ParseRequest();
    
// ************** Getters *******************

    int     getMethdType() const;
    int     getFdSocket() const;
    int     getError() const;
    int 	*getTransferMode();

    std::string	getBody(void) const;

    std::string	getChunkedBodySize(void) const;
    const std::string&  getOldPath() const;
    const std::string&  getNewPath() const;

// ************* Debug ****************
    void    printRequest();

private :
		class RequestException : public std::exception
		{
			private :
				std::string	error;

			public :
				RequestException(std::string error);
				virtual ~RequestException(void) throw();
				const char *what(void) const throw();
		};

};
