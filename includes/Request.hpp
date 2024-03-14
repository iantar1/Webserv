/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:30:58 by iantar            #+#    #+#             */
/*   Updated: 2024/03/14 03:49:48 by iantar           ###   ########.fr       */
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
// *** useless Constructor , copy constructor and assingment operator
    Request();
    Request(const Request&);
    Request& operator=(const Request&);

private:
    VirtualServer                       *Vserver;

    std::map<std::string, std::string>	Header;
	std::vector<std::string>			RequestLine;// this line: GET /hello.htm HTTP/1.1
    Location                            *location;

	char								buf[BUF_SIZE];

    int                                 SocketFd;
    int                                 ErrorFlag; // when you face an error , set a Macro to it 
    int                                 bytesRead;
	int									MethodType;
    int                                 TransferMode;  // Chuncked() or normal(content length)
    int                                 lastCharHederIndex;

    std::string                         RequestHeader;
    std::string                         chunkedBodySize;
    std::string                         body;
    std::string                         newPath;
    std::string                         oldPath;
    std::string                         HeaderReq;
    std::string                         location_str; // ! (location in configFile) don't forget to set it to it's value from ConfigFile

    bool                                doneServing;
    bool                                doneHeaderReading;

// *************  static attrebuites **************
    static std::string					Methods[3];
	static std::string					validChars;


public:
// ************ Constructor destructor ****************
    Request(int,  VirtualServer *);
    ~Request();


private:
// ***************** Private Methodes **************
    void	    SetNewPath();
    bool	    ReadCheckHeader();
    void	    saveFirstChuckBody();
    void	    storeBody();
    void	    WhichMethod(const std::string&);
    std::string skipLeadingWhitespace(const std::string&);

// ***************** Error checking Method **********
    void	URI_Checking(const std::string&);
    bool	URI_ValidLength(const std::string&) const;
    bool    URI_ValidChar(const std::string&) const;
    bool	URI_ValidLocation(const std::string&);
    void	checkValidMethod();
    void	checkValid_GET_Header();
    void	checkValid_POST_Header();
    void	checkValid_DELETE_Header();
    void	checkValidHeader();
    void	httpVersionCheck(const std::string&);

public:
// ********** Public Methods ************
    void    ParseRequest();
    void    readHeader(const std::string&, size_t);
	void	storeHeader(const std::string&);
	void	storeRequestLine(const std::string&);
	void	storeData(const std::string&);
    void	ReadRequest();
    
// ************** Getters *******************

    int     getMethdType() const;
    int     getFdSocket() const;
    int     getError() const;
    int     getTransferMode();

    bool    getDoneServing(void) const;
    bool    getDoneHeaderReading() const;

    const std::string                           &getBody(void) const;
    const std::string                           &getChunkedBodySize(void) const;
    const std::string                           &getOldPath() const;
    const std::string                           &getNewPath() const;
    const std::map<std::string, std::string>    &getRequest() const;
    

// ************* Setters ****************
    void    setDoneServing();
    void    setDoneHeaderReading();
    void    setLocation_str(std::string);
    void	setFlagError(int, const std::string&);

// ************* Debug ****************
    void    printRequest();

};
