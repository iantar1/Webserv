/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/04/01 12:41:09 by iantar           ###   ########.fr       */
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

std::string Response::getExtention(const std::string &_uri) const
{
	std::string result;

	for (int i = _uri.size() - 1; i >= 0; i--)
	{
		result += _uri[i];
		if (_uri[i] == '.')
			break;
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

std::string Response::getScriptName(const std::string &_uri)
{
	std::string result;

	for (int i = _uri.size() - 1; i >= 0; i--)
	{
		if (_uri[i] == '/')
			break;
		result += _uri[i];
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

std::string str_to_upper(const std::string &str)
{
	std::string res;

	for (size_t i = 0; i < str.size(); i++)
	{
		res += std::toupper(str[i]);
	}
	return (res);
}

std::string change_a_to_b(const std::string &str, char a, char b)
{
	std::string res;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == a)
			res += b;
		else
			res += str[i];
	}
	return (res);
}

std::string buildCgiMetaVariables(const std::string &key, const std::string &value)
{
	std::string res;

	res = change_a_to_b(key, '-', '_');
	res = str_to_upper(res);
	res += '=';

	if (key != "content-length" && key != "content-type")
		res = "HTTP_" + res;
	return (res + value);
}

char **Response::setCgiEnvironment(const std::string &_uri)
{
	char **env;

	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=CGI");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + _uri);
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + _uri);					   // * The name of the CGI script
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
	if (freopen(input_file.c_str(), "r", stdin) == NULL)
		exit(EXIT_FAILURE); // ! INTERNAL_SERVER_ERROR
}

void Response::redirectCgiOutput()
{
	if (freopen(output_file.c_str(), "w", stdout) == NULL)
	{
		std::cerr << "freopen faild\n";
		exit(EXIT_FAILURE); // ! INTERNAL_SERVER_ERROR
	}
}

bool Response::isCGI(const std::string &_uri)
{
	if (doneCGI == true)
	{
		return (false);
	}
	for (size_t i = 0; i < cgiExtention->size(); i++)
	{
		if (_uri.find(cgiExtention[i]) == _uri.size() - cgiExtention[i].size())
			return (validCGI(getExtention(_uri)));
	}
	return (false);
}

bool Response::validCGI(const std::string &extention)
{
	const LocationBlock &loc = request->getLocation();

	if ((loc.getCgiPaths()).find(extention) != (loc.getCgiPaths()).end())
		return (true);

	return (false);
}

void Response::set_args(char **args, const std::string &_uri)
{
	std::string extention;

	extention = getExtention(_uri);
	args[0] = (char *)request->getCgiPath(extention).c_str(); // !before  using this check if extention exist (.sh .php .py)
	args[1] = (char *)(getCgiFileRoot() + _uri).c_str();	  //(char *)(request->getNewPath().c_str());
	args[2] = NULL;
}

void Response::parseStoreCgiOutHeader(std::string header)
{
	std::string line;
	std::string key;
	std::string value;
	size_t pos;
	size_t p;

	while ((pos = header.find("\r\n")) != std::string::npos)
	{
		if ((p = header.find(":")) != std::string::npos)
			key = header.substr(0, p);
		else
			break;
		value = skipLeadingWhitespace(header.substr(p + 1, pos));
		//  store key value here, using your map
		if (key.compare("Set-Cookie") == 0)
			this->cookie = value;
		if (key.compare("Content-type") == 0)
			this->contentType_cgi = value;
		if (pos >= header.size() - 2)
			break;
		header = header.substr(pos + 2);
	}
}

// parse the cgi output, and extract the header from it.
void Response::extractCgiMetadata()
{
	char buf[1024];
	std::string data;
	std::string header;
	std::string body;
	ssize_t readByte;
	size_t pos;

	int fd = open(output_file.c_str(), O_RDWR, 0666);
	if (fd < 0)
	{
		std::cerr << "file can't open\n";
		return;
	}

	while ((readByte = read(fd, buf, 1024)) != 0)
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
	std::ofstream fi(output_file.c_str());
	fi << body;
	// parse header, and store it
	parseStoreCgiOutHeader(header);
	close(fd);
}

std::string Response::getCgiFileRoot()
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
		request->setFlagErrorWithoutThrow(BAD_GATEWAY, "bad gateway");
	return (status);
}

void Response::cgi_Handler(const std::string &_uri)
{
	char *args[3];
	char **env;
	pid_t pid;
	int status;

	env = setCgiEnvironment(_uri);
	set_args(args, _uri);
	// print_CGI_env(env);
	output_file = RandomName();
	if (request->getMethdType() == POST)
	{
		input_file = this->uploadedFileName;
	}
	pid = fork();
	if (pid == 0)
	{
		alarm(2);
		if (request->getMethdType() == POST)
			redirectCgiInput();
		redirectCgiOutput();
		// The CGI should be run in the correct directory for relative path file access.
		if (chdir(getCgiFileRoot().c_str()) == -1)
			exit(EXIT_FAILURE);
		execve(args[0], args, env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	doneCGI = true;
	delete[] env;
	if (chechStatus(status))
		return;
	extractCgiMetadata();
	request->setPath(output_file);
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
