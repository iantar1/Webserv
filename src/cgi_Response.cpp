/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 19:01:52 by iantar           ###   ########.fr       */
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

char **Response::setCgiEnvironment()
{
	char **env;

	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=CGI");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + request->getURI());		   // * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->getNewPath());  // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + request->getNewPath());		   // * path for cgi script

	const std::map<std::string, std::string> &headers = request->getHeaders();
	std::map<std::string, std::string>::const_iterator it = headers.begin();

	for (; it != headers.end(); ++it)
	{
		CgiEnvironment.push_back(buildCgiMetaVariables(it->first, it->second));
	}

	env = new char *[CgiEnvironment.size() + 1];
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

void Response::redirectCgiInput()
{
	if (freopen(input_cgi.c_str(),"r",stdin) == NULL)
	{
		std::cerr << "freopen faild\n";	
		exit(EXIT_FAILURE);
	}
}

void Response::redirectCgiOutput()
{
	if (freopen(output_cgi.c_str(),"w",stdout) == NULL)
	{
		std::cerr << "freopen faild\n";
		exit(EXIT_FAILURE);
	}
}

bool Response::isCGI()
{
	if (request->location.getCgiEnable() == false)
	{
		return (false);
	}
	if (doneCGI == true)
	{
		return (false);
	}
	for (size_t i = 0; i < cgiExtention->size(); i++)
	{
		if (uri.find(cgiExtention[i]) == uri.size() - cgiExtention[i].size())
			return (validCGI(getExtention()));
	}
	return (false);
}

bool Response::validCGI(const std::string &extention)
{
	const LocationBlock &loc = request->getLocation();

	if ((loc.getCgiPaths()).find(extention) != (loc.getCgiPaths()).end())
	{
		
		return (true);
	}
	return (false);
}

void Response::set_args(char **args)
{
	std::string extention;

	extention = getExtention();
	args[0] = (char *)request->getCgiPath(extention).c_str(); // !before  using this check if extention exist (.sh .php .py)
	args[1] = (char *)(request->getNewPath().c_str());		  //(char *)inFile.c_str();						  // ! use access to check if the file exist
	args[2] = NULL;
}

void Response::parseStoreCgiOutHeader(std::string header)
{
	std::string line;
	std::string key;
	std::string value;
	size_t		pos;
	size_t		p;

	cgiResponse();// default key value
	while ((pos = header.find("\r\n")) != std::string::npos)
	{
		if ((p = header.find(":")) != std::string::npos)
			key = str_to_upper(header.substr(0, p));
		else
			break;
		value = skipLeadingWhitespace(header.substr(p + 1, pos - (p + 1))); // one is for :

		cgiResponseHeaders[key] = value;
		if (pos >= header.size() - 2)
			break;
		header = header.substr(pos + 2);
	}
}

void Response::extractCgiMetadata()
{
	char buf[1024];
	std::string data;
	std::string header;
	std::string body;
	ssize_t readByte;
	size_t pos;

	std::ifstream	inFile;

	inFile.open(output_cgi.c_str());

	if (!inFile.is_open())
	{
		std::cerr << "file can't open\n";
		return;
	}

	while ((readByte = inFile.read(buf, BUF_SIZE).gcount()) != 0)
	{
		if (readByte < 0)
		{
			std::cerr << "read fails\n";
			return;
		}
		data += std::string(buf, readByte);
	}
	if ((pos = data.find("\r\n\r\n")) != std::string::npos)
	{
		header = data.substr(0, pos + 2); // add 2 to save \r\n
		pos = pos + 4;
	}
	else
		pos = 0;
	if (pos < data.size())
		body = data.substr(pos);
	std::ofstream fi(output_cgi.c_str());
	fi << body;
	fi.close();
	parseStoreCgiOutHeader(header);

	inFile.close();
}

std::string Response::getCgiFileRoot()//! xx
{
	std::string root;
	size_t pos;

	root = request->getNewPath();
	pos = root.rfind("/");
	return (root.substr(0, pos + 1));
}

bool Response::chechStatus(int status)
{
	if (status == 14)
		request->setFlagErrorWithoutThrow(GATEWAY_TIMEOUT, "GATEWAY TIMEOUT");
	else if (status)
		request->setFlagErrorWithoutThrow(INTERNAL_ERR, "INTERNAL_ERR");
	return (status);
}

bool Response::cgi_Handler()
{
	if (!this->preCGI)
	{
		char *args[3];

		this->status = 0;
		this->env = setCgiEnvironment();
		set_args(args);

		output_cgi = RandomName();
		if (request->getMethdType() == POST)
			input_cgi = this->uploadedFileName;
		startCgiTime = time(NULL);
		childToKill = true;
		this->pid = fork();

		if (this->pid == 0)
		{
			if (request->getMethdType() == POST)
				redirectCgiInput();
			redirectCgiOutput();
			if (chdir(getCgiFileRoot().c_str()) == -1)
				exit(EXIT_FAILURE);
			execve(args[0], args, this->env);
			delete[] this->env;
			exit(EXIT_FAILURE);
		}
		this->preCGI = true;
		delete[] this->env;
	}
	if (time(NULL) - startCgiTime > TIMEOUT)
	{
		this->status = 14;
		if (kill(pid, SIGKILL) == 0)
        {
            wait(NULL);
            sleep(1);
        }
		else
        {
            std::cerr << "failed to kill child\n";
        }
		childToKill = false;
	}
	if (waitpid(this->pid, &this->status, WNOHANG) == 0)
		return (false);// ! return false if it dosen't finish
	request->setPath(output_cgi);// ! you should know about this nabboune
	this->doneCGI = true;
	if (chechStatus(this->status))
	{
		return (true);// ! return true as well
	}
	extractCgiMetadata();
	return (true);
}

// ************ Getters *********************

const Request *Response::getRequest() const
{
	return (request);
}
// read the subject
// cgi for post
// he CGI should be run in the correct directory for relative path file access
