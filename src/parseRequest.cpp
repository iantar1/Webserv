/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:00:58 by iantar            #+#    #+#             */
/*   Updated: 2024/02/15 11:12:44 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "parseRequest.hpp"

std::string parseRequest::Methods[] = {"POST", "GET", "DELETE"};

parseRequest::parseRequest(int clientSocket) : clientSocket(clientSocket), MethodType(0),ReadingData(0) 
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
// create getTimeOfDay() , return a strin ddmmyy

std::string	parseRequest::generateRandomFileName(size_t length) const// add the dtae of today
{
    const std::string   str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t        strSize = str.size();
    std::string         fileNmae;

    srand(static_cast<unsigned int>(time(NULL)));
    for (size_t i = 0; i < length; i++)
    {
        fileNmae += str[rand() % strSize];
    }
    return ("DataBase/" + fileNmae);
}

void	parseRequest::ReadStoreBody()
{
    int fd;
    int rdSize;

    BodyfileNmae = generateRandomFileName(6);
    fd = open(BodyfileNmae.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd < 0)
        throw std::runtime_error("file can't open");
    write(fd, Body.c_str(), Body.size());
    ReadingData = Body.size();
    while ((rdSize = read(clientSocket, buf, BUF_SIZE)) != 0)
    {
        write(fd, buf, rdSize);
        ReadingData += rdSize;
        if (ReadingData == ContentLength)
            break;
    } 
    close(fd);
}

void	parseRequest::storeData(const std::string& dataRequest, size_t index)
{
	std::istringstream iss(dataRequest);
    std::string line;

    // ;
    // std::cout << "lplplp\n";
    
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
	//storeHeader(line);
    ContentLength = atoi((Header["Content-Length:"]).c_str());
    Body = dataRequest.substr(index);
	// while (std::getline(iss, line, '\n')) // if there is a body , mabye you need to read again
	// {
	// }
    if (MethodType == POST)
    {
        ReadStoreBody();
    }
    else if (MethodType == GET)
    {
        parseRequest::getMethode();
        
    }
}



void    parseRequest::getMethode()
{
    std::size_t fileSize;
    const char* filename = "DataBase/video.mp4";
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0) {
        fileSize = file_stat.st_size;
    }
    else
    {
        throw std::runtime_error("Error");
    }
    std::stringstream sNum;
    sNum << fileSize;
    std::cout << fileSize << "\n";
    std::string title = "HTTP/1.1 200 OK\r\nContent-Type: video/mp4\r\nContent-Length: " + sNum.str() + "\r\n\r\n";
    if (write(clientSocket, title.c_str(), title.size()) == -1)
    {
        throw std::runtime_error("send syscall failed, first");
    }
    int size;
    int fd;
    fd = open("DataBase/video.mp4", O_RDONLY, 0666);
    if (fd < 0)
        throw std::runtime_error("file can't open");
    char buf[BUF_SIZE];
    bzero(buf, BUF_SIZE);
    while ((size = read(fd, buf, BUF_SIZE)) != 0)
    {
        if (size == -1)
            throw std::runtime_error("read error\n");
        //ssize_t sizeSend = 0;
        //while (sizeSend != size)
        //{
        ssize_t tmp = write(clientSocket, buf, size);
        if (tmp != size)
            throw std::runtime_error("Tkshbila");
        if (tmp == -1)
            throw std::runtime_error("write syscall failed");
           // sizeSend += tmp;
        //}
        bzero(buf, BUF_SIZE);
    }
    close(fd);
}

void	parseRequest::readData(void)
{
    int bytesRead;
    size_t index;
    std::string data;

    while ((bytesRead = read(clientSocket, buf, BUF_SIZE)) > 0) {
        data.append(buf, bytesRead);
        if ((index = data.find("\r\n\r\n")) != std::string::npos) {
	        storeData(data, index + 4);
            break;
        }
    }
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
