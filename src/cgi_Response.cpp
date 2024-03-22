/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/03/22 04:16:46 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"
#include "../includes/headers.hpp"

// ******* main method ************
// ! check path of the cgi
// ! parce extention .py .sh .php ...

// * you should map cgi map[.sh] = path

std::string Response::getExtention() const
{
	std::string result;

	for (int i = uri.size() - 1; i >= 0; i--)
	{
		result += uri[i];
		if (uri[i] == '.')
			break;
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

std::string Response::getScriptName()
{
	std::string result;
	std::string uri;

	uri = request->getURI();
	for (size_t i = uri.size() - 1; i >= 0; i--)
	{
		if (uri[i] == '/')
			break;
		result += uri[i];
	}
	return (result);
}

void Response::print_CGI_env(char **env)
{
	for (size_t i = 0; i < 8; i++)
	{
		std::cout << env[i] << std::endl;
	}
}

void Response::setCgiEnvironment()
{
	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=200");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + request->getURI());		   // * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->getNewPath());  // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + request->getNewPath());		   // * path for cgi script
}

std::string Response::RandomName()
{
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string result = request->getLocation()->getRoot();

	srand(time(0));
	for (int i = 0; i < 5; i++)
		result += str[rand() % str.size()];
	result += ".txt";
	return result;
}

void	Response::redirectCgiInput()
{
	int fd_in;

	if ((fd_in = open(inputFile.c_str(), O_RDONLY | O_CREAT, 0666)) == -1)
		throw std::runtime_error("open system call failed");
	
	if (dup2(fd_in, 0) == -1)
		throw std::runtime_error("dup2 system call failed");
	close(fd_in);
}

void	Response::redirectCgiOutput()
{
	int fd_out;

	if ((fd_out = open(outputFile.c_str(), O_RDONLY | O_CREAT, 0666)) == -1)
		throw std::runtime_error("open system call failed");// ! set a INTERNAL_SERVER_ERROR 
	
	if (dup2(fd_out, 0) == -1)
		throw std::runtime_error("dup2 system call failed");
	close(fd_out);
}

void Response::storeUserInput()
{
	std::ofstream input;

	input.open("DataBase/infile");
	if (request->getMethdType() == POST)
		input << this->request->getBody();
	// if (request->getMethdType() == GET)
	// 	input << this->request->getQueryString();
	input.close();
}

std::string cgiExtention[] = {".sh", ".py", ".php"}; // ! static varible

bool Response::isCGI()
{
	if (doneCGI)
		return (false);
	for (size_t i = 0; i < cgiExtention->size(); i++)
	{
		if (uri.find(cgiExtention[i]) == uri.size() - cgiExtention[i].size())
			return (validCGI(getExtention()));
	}
	return (false);
}

bool Response::validCGI(const std::string &extention)
{
	const Location *loc = request->getLocation();

	if ((loc->getCGI_Map()).find(extention) != (loc->getCGI_Map()).end())
		return (true);

	return (false);
}

void Response::cgi_Handler()
{
	std::string	_outFile;
	std::string	extention;
	char		*args[3];
	char		*env[9];
	pid_t		pid;
	int 		status;

	setCgiEnvironment();
	extention = getExtention();
	std::cout << extention << "\n";
	args[0] = (char *)request->getCgiPath(extention).c_str(); // !before  using this check if extention exist (.sh .php .py)
	args[1] = (char *)(request->getNewPath().c_str());		  //(char *)inFile.c_str();						  // ! use access to check if the file exist
	args[2] = NULL;
	std::cout << "arg[0]: " << args[0] << std::endl;
	std::cout << "arg[1]: " << args[1] << std::endl;
	// std::cout << "arg[2]: "<< args[2] << std::endl;

	_outFile = RandomName();
	storeUserInput();
	for (size_t i = 0; i < 8; i++)
	{
		env[i] = (char *)CgiEnvironment[i].c_str();
	}
	env[8] = NULL;
	print_CGI_env(env);
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
		close(fd_out);
		close(fd_in);
		execve(args[0], args, env);
	}
	waitpid(pid, &status, 0);
	doneCGI = true;
	if (!status)
	{
		std::cout << "status: " << status << std::endl;
		request->setPath(_outFile);
	}
	// * change the file path to _outfile
	// ! check status
	// ! tuimeout
	std::cout << "output: " << _outFile << std::endl;
}

// ************ Getters *********************

const Request *Response::getRequest() const
{
	return (request);
}