/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/03/19 03:22:27 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"
#include "../includes/headers.hpp"

// ******* main method ************
// ! check path of the cgi
// ! parce extention .py .sh .php ...

// * you should map cgi map[.sh] = path

std::string Response::getExtention(const std::string &filePath) const
{
	std::string result;

	for (int i = filePath.size() - 1; i >= 0; i--)
	{
		result += filePath[i];
		if (filePath[i] == '.')
			break;
	}
	std::reverse(result.begin(), result.end());
	std::cout << "file : " << result << "\n";
	return (result);
}

//  	"REQUEST_METHOD="
//     "SERVER_PROTOCOL="
//     "QUERY_STRING="
//     "SCRIPT_NAME="
//     "PATH_INFO="

//     "SCRIPT_FILENAME="
//     "REDIRECT_STATUS=200"
// 	    "REQUEST_URI="
std::string	Response::getScriptName()
{
	std::string	result;
	std::string	uri;

	uri = request->getURI();
	std::cout << "fr: " << uri << "\n";
	for (size_t i = uri.size() - 1; i >= 0; i--)
	{
		if (uri[i] == '/')
			break ;
		result += uri[i];
	}
	return (result);
}

void Response::print_CGI_env()
{
	for (size_t i = 0; i < CgiEnvironment.size(); i++)
	{
		std::cout << CgiEnvironment[i] << std::endl;
	}
}

void Response::setCgiEnvironment()
{
	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=200");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString());// !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + request->getURI());// * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->getNewPath()); // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + request->getNewPath());// * path for cgi script
}

std::string	RandomName()
{
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string	result = "/tmp/";

	srand(time(0));
	for (int i = 0; i < 5; i++)
		result += str[rand() % str.size()];
	return result;
}

void	Response::storeUserInput()
{
	std::ofstream	input;

	input.open("DataBase/infile");
	if (request->getMethdType() == GET)
		input << this->request->getQueryString();
	if (request->getMethdType() == POST)
		input << this->request->getBody();
	input.close();
}

void Response::cgi_Handler(const std::string &inFile)
{
	std::string _outFile;
	std::string extention;
	char *args[3];
	char *env[8];
	pid_t pid;

	setCgiEnvironment();
	extention = getExtention(request->getURI());
	std::cout << extention << "\n";
	args[0] = (char *)request->getCgiPath(extention).c_str(); // !before  using this check if extention exist (.sh .php .py)
	args[1] = (char *)(request->getNewPath().c_str());//(char *)inFile.c_str();						  // ! use access to check if the file exist
	args[2] = NULL;
	std::cout << "arg[0]: "<< args[0] << std::endl;
	std::cout << "arg[1]: "<< args[1] << std::endl;
	// std::cout << "arg[2]: "<< args[2] << std::endl;

	_outFile = RandomName();
	storeUserInput();
	for (size_t i = 0; i < 8; i++)
	{
		env[i] = (char *)CgiEnvironment[i].c_str();
	}
	print_CGI_env();
	pid = fork();
	if (pid == 0)
	{
		int fd_out = open(_outFile.c_str(), O_WRONLY | O_CREAT, 0666);
		int fd_in = open("DataBase/infile", O_RDONLY | O_CREAT, 0666);
		if (fd_in < 0 || fd_out < 0)
		{
			std::cout << "can'topen\n";
			exit(19); // ! use a macro
		}
		dup2(fd_out, 1);
		dup2(fd_in, 0);
		// close(fd);
		execve(args[0], args, NULL);
	}
	int status;
	waitpid(pid, &status, 0);
	std::cout << "child done\n";
	if (status)
	{
		std::cout << "status: " << status << std::endl;
		exit(1);
	}
	// * change the file path to _outfile
	// ! check status
	// ! tuimeout
	std::cout << "output: " << _outFile << std::endl;
	(void)inFile;
}
