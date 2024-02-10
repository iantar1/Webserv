/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:01:01 by iantar            #+#    #+#             */
/*   Updated: 2024/02/10 12:47:41 by iantar           ###   ########.fr       */
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
	int 								Sockfd;
	int									MethodType;
	char								buf[BUF_SIZE];
	int									readSize;
	std::vector<std::string>			RequestLine;
	std::map<std::string, std::string>	Header;
	std::string							Body;
	static std::string					Methods[3];
	std::string							BodyfileNmae;
	

private:
	parseRequest();

	void		storeData(const std::string&);
	void		storeHeader(const std::string&);
	void		storeRequestLine(const std::string&);
	void		ReadStoreBody();
	std::string	generateRandomFileName(size_t) const;

public:
// parametrize constructor, copy constructor, copy assignement operator and destructor
	parseRequest(int);
	parseRequest(const parseRequest&);
	parseRequest& operator=(const parseRequest&);
	~parseRequest();

	void	readData();
	void 	printData();
};

