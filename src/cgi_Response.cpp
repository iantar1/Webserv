/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/04/01 11:18:31 by iantar           ###   ########.fr       */
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
	for (int i = 0; env[i]; i++)
	{
		std::cerr << env[i] << std::endl;
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

char **Response::setCgiEnvironment()
{
	char **env;

	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=CGI");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	// CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + request->getURI());		   // * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->getNewPath());  // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + request->getNewPath());		   // * path for cgi script

	const std::map<std::string, std::string> &headers = request->getHeaders();
	std::map<std::string, std::string>::const_iterator it = headers.begin();
	if ((it = headers.find("cookie")) != headers.end())
	{
		CgiEnvironment.push_back(buildCgiMetaVariables(it->first, it->second));
	}
	// ! check this later
	// for (; it != headers.end(); ++it)
	// {
	// 	CgiEnvironment.push_back(buildCgiMetaVariables(it->first, it->second));
	// }

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
	// std::cout << "getRoot: " << request->getLocation().getRoot() << "\n";

	if (result[result.size() - 1] != '/')
		result += '/';
	srand(time(0));
	for (int i = 0; i < 5; i++)
		result += str[rand() % str.size()];
	result += ".txt";
	// std::cout << "result: " << result << "\n";
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
// ! NOT FOUNT CGI
bool Response::isCGI()
{
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
		return (true);

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
	size_t pos;
	size_t p;

	while ((pos = header.find("\r\n")) != std::string::npos)
	{
		if ((p = header.find(":")) != std::string::npos)
			key = header.substr(0, p);
		else
			break;
		value = skipLeadingWhitespace(header.substr(p + 1, pos - (p + 1))); // one is for :

		cgiResponseHeaders[str_to_upper(key)] = value;
		if (pos >= header.size() - 2)
			break;
		header = header.substr(pos + 2);
	}
}

// parse the cgi output, and extract the header from it.
// void Response::extractCgiMetadata()
// {

// }
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
			close(fd);
			errorPage(INTERNAL_ERR);
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
	parseStoreCgiOutHeader(header);

	std::map<std::string, std::string>::iterator it = this->cgiResponseHeaders.find("STATUS");
	if (it != this->cgiResponseHeaders.end())
		this->response += "HTTP/1.1 " + it->second + "\r\n";
	it = this->cgiResponseHeaders.begin();
	while (it != this->cgiResponseHeaders.end())
	{
		if (it->first == "STATUS")
		{
			it++;
			continue;
		}
		else if (it->first == "CONTENT-LENGHT")
			this->response += it->first + ": " + ToString(body.size()) + "\r\n";
		else
		{
			this->response += it->first + ": " + it->second + "\r\n";
		}
		it++;
	}

	this->response += "\r\n" + body;
	if (this->response.empty())
		errorPage(BAD_REQUEST);
	this->request->setDoneServing();
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
	// if (status == 14)
	// 	request->setFlagErrorWithoutThrow(GATEWAY_TIMEOUT, "GATEWAY TIMEOUT");
	if (status)
		request->setFlagErrorWithoutThrow(BAD_GATEWAY, "bad _gateway");
	return (status);
}
void Response::cgi_Handler()
{
	char *args[3];
	char **env;
	pid_t pid;
	int status = 0;

	// if (access())
	env = setCgiEnvironment();
	set_args(args);

	output_file = RandomName();
	std::cout << "body: " << request->getBody() << "\n";
	if (request->getMethdType() == POST)
	{

		input_file = this->uploadedFileName;
		std::cout << "uploadedFileName: " << uploadedFileName << "\n";
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
		print_CGI_env(env);
		execve(args[0], args, env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &status, 0);
	doneCGI = true;
	delete[] env;
	if (chechStatus(status))
		return;
	extractCgiMetadata();
	if (request->getMethdType() == GET)
		request->setPath(output_file);
}

// ************ Getters *********************

const Request *Response::getRequest() const
{
	return (request);
}

// void	timeHandler()
// {
// 	if ()
// 	{
// 		kill(pid_t pid, int sig);
// 	}
// }

// void Response::cgi_Handler()
// {
// 	char *args[3];
// 	char **env = NULL;
// 	// pid_t pid = -1;
// 	int status = -100;

// 	if (access(request->getNewPath().c_str(), F_OK))
// 	{
// 		errorPage(NOT_FOUND);
// 		this->request->setDoneServing();
// 		return;
// 	}
// 	// generate  files filename if nessetry and one time
// 	if (request->getMethdType() == POST && input_file.empty())
// 	{
// 		input_file = this->uploadedFileName;
// 	}
// 	if (output_file.empty())
// 		output_file = RandomName();

// 	if (cgi_state == -1) // enter firt time
// 	{
// 		env = setCgiEnvironment();
// 		set_args(args);
// 		cgi_timer = time(NULL);
// 		print_CGI_env(env);
// 		pid = fork();
// 		if (pid < 0)
// 		{
// 			errorPage(INTERNAL_ERR);
// 			this->request->setDoneServing();
// 			delete[] env;
// 			return;
// 		}
// 		cgi_state = 0; // to not unter next time
// 	}
// 	if (pid == 0 && !cgi_state) // parent pid will never equal to 0
// 	{
// 		if (request->getMethdType() == POST)
// 			redirectCgiInput();
// 		redirectCgiOutput();
// 		// The CGI should be run in the correct directory for relative path file access.
// 		if (chdir(getCgiFileRoot().c_str()) == -1)
// 			((delete[] env), exit(EXIT_FAILURE));
// 		std::cerr << "arg: " << args[0] << " " << args[1] << "\n";
// 		std::cerr << YELLOW << "in the child, pid: " << pid << "\n";
// 		execve(args[0], args, env);
// 		((delete[] env), exit(EXIT_FAILURE));
// 	}
// 	waitpid(pid, &status, 0);
// 	exit(1);
// 	cgi_state = 2; // which state we are
// 	delete[] env;
// 	pid_t wpid = waitpid(pid, &status, WNOHANG);
// 	if (time(NULL) - cgi_timer > 2)
// 	{
// 		std::cout << "time out\n";
// 		if (kill(pid, SIGKILL) == 0)
// 		{
// 			std::cerr << RED << "child killed\n"
// 					  << RESET;
// 		}
// 		// waitpid(pid, NULL, 0);
// 		request->setFlagErrorWithoutThrow(GATEWAY_TIMEOUT, "GATEWAY TIMEOUT");
// 		return;
// 	}
// 	// std::cout << "status :" << status << "\n";
// 	if (!wpid)
// 		return;
// 	if (status == -100)
// 		return;
// 	std::cout << "stusts: " << status << "\n";
// 	doneCGI = true;
// 	cgi_state = 1;
// 	if (chechStatus(status))
// 		return;
// 	// std::cout << "Output file : " << output_file << std::endl;
// 	extractCgiMetadata();
// 	// request->setPath(output_file);
// 	// ! tuimeout
// }

/*
 */

// read the subject
// cgi for post
// he CGI should be run in the correct directory for relative path file access
