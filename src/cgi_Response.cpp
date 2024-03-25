/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/03/25 09:07:54 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"
#include "../includes/headers.hpp"

// ******* main method ************
// ! check path of the cgi
// ! parce extention .py .sh .php ...

// * you should map cgi map[.sh] = path

static std::string cgiExtention[] = {".sh", ".py", ".php"}; // ! static varible

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
	for (int i = uri.size() - 1; i >= 0; i--)
	{
		if (uri[i] == '/')
			break;
		result += uri[i];
	}
	return (result);
}

void Response::print_CGI_env(char **env)
{
	for (int i = 0; i < 8; i++)
	{
		std::cout << env[i] << std::endl;
	}
}

std::string	str_to_upper(const std::string& str)
{
	std::string	res;

	for (size_t i = 0; i < str.size(); i++)
	{
		res += std::toupper(str[i]);
	}
	return (res);
}

std::string	change_a_to_b(const std::string& str, char a, char b)
{
	std::string	res;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == a)
			res += b;
		else
			res += str[i];
	}
	return (res);
}

std::string	buildCgiMetaVariables(const std::string& key, const std::string& value)
{
	std::string	res;

	res = change_a_to_b(key, '-', '_');
	res = str_to_upper(res);
	res += '=';
	
	if (key != "content-length" && key != "content-type")
		res = "HTTP_" + res;
	return (res + value);
}

char ** Response::setCgiEnvironment()
{
	char ** env;

	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=CGI");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + request->getURI());		   // * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->getNewPath());  // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + request->getNewPath());		   // * path for cgi script

	const std::map<std::string, std::string>& headers = request->getHeaders();
	std::map<std::string, std::string>::const_iterator it = headers.begin();
	
	for (;it != headers.end(); ++it)
	{
		CgiEnvironment.push_back(buildCgiMetaVariables(it->first, it->second));
	}
	
	env = new char*[CgiEnvironment.size() + 1];
	for (size_t i = 0; i < CgiEnvironment.size(); i++)
	{
		env[i] = (char *)CgiEnvironment[i].c_str();
	}
	env[CgiEnvironment.size()] = NULL;
	return (env);
}

std::string Response::RandomName()
{
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string result = request->getLocation().getRoot();

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
	const LocationBlock& loc = request->getLocation();

	if ((loc.getCgiPaths()).find(extention) != (loc.getCgiPaths()).end())
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
	char		**env;
	pid_t		pid;
	int 		status;

	std::cout << RED << "CGI" << RESET << std::endl;
	env = setCgiEnvironment();
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
		redirectCgiOutput();
		// if (chdir() == -1) // check subject////////////////////
		// 	exit(EXIT_FAILURE);
		execve(args[0], args, env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	doneCGI = true;
	if (status)
	{
		// ! set error flag
		// !tmeout
	}
	if (!status)
	{
		request->setPath(output_file);
	}
	delete [] env;
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
