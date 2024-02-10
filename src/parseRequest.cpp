/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:00:58 by iantar            #+#    #+#             */
/*   Updated: 2024/02/10 13:08:56 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "parseRequest.hpp"

std::string parseRequest::Methods[] = {"POST", "GET", "DELETE"};

parseRequest::parseRequest(int sockfd) : Sockfd(sockfd), MethodType(0)
{
}

void	parseRequest::storeHeader(const std::string& line)
{
	std::string	key;
	std::string	value;
	size_t		index;

	index = line.find(":");
	key = line.substr(0, index + 1);
	value = line.substr(index + 1);
	Header[key] = value;
}

/// @brief sotore Request line and set MethodType to its value if valid otherwise throw an exeption.
/// @param firt line of a client request 

void	parseRequest::storeRequestLine(const std::string& line)
{
	std::stringstream	sstr(line);
	std::string			word;

	while (sstr >> word)
	{
		RequestLine.push_back(word);
	}
	for (int i = 0; i < 3; i++)
	{
		if (Methods[i] == RequestLine[0])
		{
			MethodType = i + 1;
			return ;
		}
	}
	throw std::runtime_error("Invalid Method");
}

std::string	parseRequest::generateRandomFileName(size_t length) const
{
    const std::string   str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t        strSize = str.size();
    std::string         fileNmae;

    srand(static_cast<unsigned int>(time(NULL)));
    for (size_t i = 0; i < length; i++)
    {
        fileNmae += str[rand() % strSize];
    }
    return (fileNmae);
}

void	parseRequest::ReadStoreBody()
{
    int fd;
    int rdSize;

    BodyfileNmae = generateRandomFileName(6);
    fd = open(BodyfileNmae.c_str(), O_RDWR | O_CREAT);
    if (fd < 0)
        throw std::runtime_error("file can't open");
    write(fd, Body.c_str(), Body.size());
    // hardcode
    while ((rdSize = read(Sockfd, buf, BUF_SIZE)) == BUF_SIZE)
    {
        write(fd, buf, rdSize);
    } 
    write(fd, buf, rdSize);
    close(fd);
}

void	parseRequest::storeData(const std::string& dataRequest)
{
	std::istringstream iss(dataRequest);
    std::string line;

	for (int i = 0; std::getline(iss, line); i++)
	{
		if (i == 0)
		{
			storeRequestLine(line);
		}
		else if (line == "\r")
		{
			break ;
		}
		else
		{
			storeHeader(line);
		}
	}
	while (std::getline(iss, line)) // if there is a body , mabye you need to read again
	{
		Body += line;
	}
    if (MethodType == POST)
    {
        ReadStoreBody();
    }
}

void	parseRequest::readData(void)
{
	readSize = read(Sockfd, buf, BUF_SIZE);

	if (readSize == -1)
		throw std::runtime_error("can't read client socket");
	storeData(std::string(buf, readSize));
	
// if you don't spesifies the size to std::string's constructor, the string will stop at '\0', 
// whish will lead you to lose a part of your body.
}

parseRequest::~parseRequest()
{
}


void parseRequest::printData()
{
    std::cout << "****************************RequestLine**************************\n";
    for (size_t i = 0; i < RequestLine.size(); i++)
    {
        std::cout << RequestLine[i] << std::endl;
    }
    std::cout << "****************************Header**************************\n";
    std::map<std::string, std::string>::iterator it;
    for (it = Header.begin(); it != Header.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
    std::cout << "****************************Body**************************\n";
    if (MethodType == POST)
    {
        std::cout <<"the Body : \n" << Body << " redsize: "<< readSize  << std::endl;
    }
    else
    {
        std::cout << "No Body" << " redsize: "<< readSize << std::endl;
    }
}
