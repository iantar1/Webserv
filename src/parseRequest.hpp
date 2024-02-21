/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:01:01 by iantar            #+#    #+#             */
/*   Updated: 2024/02/19 14:12:00 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "../includes/headers.hpp"
# include "../includes/macros.hpp"
# define BUF_SIZE 1024
# define COLON 58
# define CARRIGE_RET 13
# define POST 1
# define GET 2
# define DELETE 3 


class parseRequest
{
private:
	int 								clientSocket;
	int									MethodType;
	char								buf[BUF_SIZE];
	int									readSize;
	std::vector<std::string>			RequestLine;
	std::map<std::string, std::string>	Header;
	std::string							Body;
	static std::string					Methods[3];
	std::string							BodyfileNmae;
	int									ReadingData;
	int									ContentLength;
	int									fdFile;


private:
	parseRequest();

	void		storeData(const std::string&, size_t);
	void		storeHeader(const std::string&);
	void		storeRequestLine(const std::string&);
	void		ReadStoreBody();
	std::string	generateRandomFileName(size_t) const;
	std::string	CurrentDate();

public:
	bool								isDone;
	bool								reading_done;
// parametrize constructor, copy constructor, copy assignement operator and destructor
	parseRequest(int);
	parseRequest(const parseRequest&);
	parseRequest& operator=(const parseRequest&);
	~parseRequest();

	void	getMethode();
	void	readData();
	void 	printData();
};

