/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nabboune <nabboune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/03/23 03:51:50 by nabboune         ###   ########.fr       */
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

void Response::setCgiEnvironment(char **env)
{
	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=200");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + request->getURI());		   // * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->getNewPath());  // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + request->getNewPath());		   // * path for cgi script

	for (size_t i = 0; i < 8; i++)
	{
		env[i] = (char *)CgiEnvironment[i].c_str();
	}
	env[8] = NULL;
}

std::string Response::RandomName()
{
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string result = request->getLocation()->getRoot();

	if (result[result.size() - 1] != '/')
		result += '/';
	srand(time(0));
	for (int i = 0; i < 5; i++)
		result += str[rand() % str.size()];
	result += ".txt";
	return result;
}

void	Response::redirectCgiInput()
{
	int fd_in;

	if ((fd_in = open(input_file.c_str(), O_RDONLY | O_CREAT, 0666)) == -1)
		exit(1);// ! INTERNAL_SERVER_ERROR 
	// std::cout << "body: " << body << std::endl;
	// if (write(fd_in, body.c_str(), body.size()) == -1)
	// 	exit(1);// ! INTERNAL_SERVER_ERROR 
	if (dup2(fd_in, 0) == -1)
		exit(1);// ! INTERNAL_SERVER_ERROR 
	close(fd_in);
}

void	Response::redirectCgiOutput()
{
	int fd_out;

	if ((fd_out = open(output_file.c_str(), O_WRONLY | O_CREAT, 0666)) == -1)
		exit(1);// ! INTERNAL_SERVER_ERROR 
	
	if (dup2(fd_out, 1) == -1)
		exit(1);// ! INTERNAL_SERVER_ERROR 
	close(fd_out);
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

void	Response::set_args(char **args)
{
	std::string	extention;

	extention = getExtention();
	args[0] = (char *)request->getCgiPath(extention).c_str(); // !before  using this check if extention exist (.sh .php .py)
	args[1] = (char *)(request->getNewPath().c_str());		  //(char *)inFile.c_str();						  // ! use access to check if the file exist
	args[2] = NULL;
	
}


void Response::cgi_Handler()
{
	char		*args[3];
	char		*env[9];
	pid_t		pid;
	int 		status;

	setCgiEnvironment(env);
	set_args(args);

	output_file = RandomName();
	if (request->getMethdType() == POST)
	{
		input_file = this->uploadedFileName;// ! you need to generate a random file , just use 
		// ! nabboune /Uplodes/ to read from
	}
	// print_CGI_env(env);
	pid = fork();
	// if (pid)
	// 	sleep(1);
	if (pid == 0)
	{
		if (request->getMethdType() == POST)
				redirectCgiInput();
		std::cout <<  "here: " << this->uploadedFileName;
		redirectCgiOutput();
		print_CGI_env(env);
		execve(args[0], args, env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	doneCGI = true;
	if (!status)
	{
		request->setPath(output_file);
	}
	// * change the file path to _outfile
	// ! check status
	// ! tuimeout
}

// ************ Getters *********************

const Request *Response::getRequest() const
{
	return (request);
}
// read the subject
// cgi for post
// he CGI should be run in the correct directory for relative path file access
