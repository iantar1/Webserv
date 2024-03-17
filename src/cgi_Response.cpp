/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/03/17 04:19:44 by iantar           ###   ########.fr       */
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

	for (size_t i = filePath.size() - 1; i >= 0; i--)
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

// void Response::setCgiEnvironment()
// {
// 	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
// 	CgiEnvironment.push_back("REDIRECT_STATUS=200");
// 	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
// 	CgiEnvironment.push_back("REQUEST_URI=" + request->getRequestURI());
// 	CgiEnvironment.push_back("QUERY_STRING=" + getQueryString());// !start mn ?
// 	CgiEnvironment.push_back("SCRIPT_NAME=" + getScriptName());
// 	CgiEnvironment.push_back("SCRIPT_FILENAME=" + getscriptFilename());
// 	CgiEnvironment.push_back("PATH_INFO=" + getPathInfo());
// }

void Response::cgi_Handler(const std::string &inFile)
{
	std::string _outFile;
	std::string extention;
	char *args[3];
	// char *env[6];
	pid_t pid;

	// setCgiEnvironment();
	extention = getExtention(inFile);
	args[0] = (char *)request->getCgiPath(extention).c_str(); // !before  using this check if extention exist (.sh .php .py)
	args[1] = (char *)inFile.c_str();						  // ! use access to check if the file exist
	args[2] = NULL;

	_outFile = "outPut~";// + generateNameFile();
	// for (int i = 0; i < 6; i++)
	// {
	// 	env[i] = (char *)CgiEnvironment[i].c_str();
	// }
	pid = fork();
	if (pid == 0)
	{
		int fd = open(_outFile.c_str(), O_WRONLY | O_CREAT, 0666);
		if (fd < 0)
		{
			std::cout << "can'topen\n";
			exit(19); // ! use a macro
		}
		std::cout << "HEE\n";
		printf("-> %s , , %s\n", args[0], args[1]);
		dup2(fd, 1);
		close(fd);
		execve(args[0], args, NULL);
	}
	int status;
	waitpid(pid, &status, 0);
	// ! check status
	// ! tuimeout
}
