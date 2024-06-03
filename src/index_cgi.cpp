/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iantar <iantar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 23:03:14 by iantar            #+#    #+#             */
/*   Updated: 2024/06/02 19:05:34 by iantar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Response.hpp"
#include "../includes/utils.hpp"
#include "../includes/headers.hpp"

// ******* main method ************
// ! check path of the cgi
// ! parce extention .py .sh .php ...

// * you should map cgi map[.sh] = path

static std::string cgiExtention[] = { ".sh", ".py", ".php" }; // ! static varible

std::string Response::getExtention(const std::string& path) const
{
	std::string result;

	for (int i = path.size() - 1; i >= 0; i--)
	{
		result += path[i];
		if (path[i] == '.')
			break;
	}
	std::reverse(result.begin(), result.end());
	return (result);
}

char** Response::setCgiEnvironment(const std::string& path)
{
	char** env;

	CgiEnvironment.push_back("SERVER_PROTOCOL=HTTP/1.1");
	CgiEnvironment.push_back("REDIRECT_STATUS=CGI");
	CgiEnvironment.push_back("REQUEST_METHOD=" + request->getMethod());
	CgiEnvironment.push_back("REQUEST_URI=" + request->getURI());
	CgiEnvironment.push_back("QUERY_STRING=" + request->getQueryString()); // !start mn ?
	CgiEnvironment.push_back("SCRIPT_NAME=" + path);		   // * The name of the CGI script
	CgiEnvironment.push_back("SCRIPT_FILENAME=" + request->location.getRoot() + path);  // * The full path to the CGI script
	CgiEnvironment.push_back("PATH_INFO=" + path);		   // * path for cgi script

	const std::map<std::string, std::string>& headers = request->getHeaders();
	std::map<std::string, std::string>::const_iterator it = headers.begin();

	for (; it != headers.end(); ++it)
	{
		CgiEnvironment.push_back(buildCgiMetaVariables(it->first, it->second));
	}

	env = new char* [CgiEnvironment.size() + 1];
	for (size_t i = 0; i < CgiEnvironment.size(); i++)
	{
		env[i] = (char*)CgiEnvironment[i].c_str();
	}
	env[CgiEnvironment.size()] = NULL;
	return (env);
}

bool	Response::setCgiNewPath()
{
	// std::string 	newPath = request->getNewPath();

	if (this->oneTime == 0)
	{
		if (path.size() && path[0] != '/')
			request->setPath(request->getNewPath() +  path);//! and if there is no / in newPath
		this->oneTime = 1;
	}
	return (true);
}

bool Response::isCGI(const std::string& path)
{
	// int oneTime = 0;
	

	if (request->location.getCgiEnable() == false)
	{
		return (false);
	}
	this->cgiIndexPath = path;
	if (doneCGI == true)
	{
		return (false);
	}
	for (size_t i = 0; i < cgiExtention->size(); i++)
	{
		if (path.find(cgiExtention[i]) == path.size() - cgiExtention[i].size())
		{
			return (validCGI(getExtention(path)) && setCgiNewPath());
		}
	}
	return (false);
}
// this becouse of (request->location.getRoot() + path) operator+()
char *ft_strdup(const std::string& str)
{
	if (str.empty())
		return (NULL);
	char * allocatedStr = new char[str.length() + 1];
	strcpy(allocatedStr, str.c_str());
	return (allocatedStr);	
}
void Response::set_args(char** args, const std::string& path)
{
	std::string extention;

	extention = getExtention(path);
	args[0] = ft_strdup((request->getCgiPath(extention))); // !before  using this check if extention exist (.sh .php .py)
	args[1] = ft_strdup((request->location.getRoot() + path)); // ! check if there is '/' or not
	args[2] = NULL;
}

std::string Response::getCgiFileRoot(const std::string& path)
{
	std::string root;
	size_t pos;

	root = request->location.getRoot() + path;
	pos = root.rfind("/");
	return (root.substr(0, pos + 1));
}

bool	Response::prePre(void)
{
	if (stat(request->getNewPath().c_str(), &this->buffer) == -1)
	{
		this->cgiResponse();
		theGetErrorNotFound();
		this->request->setDoneServing();
		return true;
	}
	if (S_ISDIR(this->buffer.st_mode))
	{
		if (request->getOldPath()[request->getOldPath().size() - 1] != '/')// ! you were using 'this->path' wich is empty , get it from request by getNewPath()
		{
			theGetRedirectionRequest();
			this->request->setDoneServing();
			this->request->oldPath += "/";
			return true;
		}
	}
	return false;
}
//! here you should index the file with th root path : root _
// bool	Response::prePre(void)
// {
// 	// std::string	pathToCheck;
// 	// std::string	path = request->getNewPath();

// 	if (stat(request->getNewPath().c_str(), &this->buffer) == -1)
// 	{
// 		theGetErrorNotFound();
// 		this->request->setDoneServing();
// 		return true;
// 	}
// 	if (S_ISDIR(this->buffer.st_mode))
// 	{
// 		if (request->getNewPath()[request->getNewPath().length() - 1] != '/')// ! you were using 'this->path' wich is empty , get it from request by getNewPath()
// 		{
// 			theGetRedirectionRequest();
// 			this->request->setDoneServing();
// 			return true;
// 		}
// 	}
// 	return false;
// }


bool Response::isThisPathExist(std::string path, std::string index)
{
	(void)index;
	if (path.size() && path[path.size() - 1] == '/')
		path = path.substr(0, path.size() - 1);
	if (index.size() > 1 && index[0] == '/')
		index = index.substr(1, index.size() - 1);
	path = path + '/' + index;
	//! Khassek tkhdem b stats() w status w dak lkhire lakhoure
	//! Access makhdamache la kane directory ==> tat3tabrou makaynche !...
	// struct stat data;
	
	// if (access(path.c_str(), F_OK))
	// {
	// 	return (false);
	// }
	// return (true);


	struct stat data;
	if (stat(path.c_str(), &data) != -1)
	{
		// if (S_ISDIR(data.st_mode) || S_ISREG(data.st_mode))
		// {
		this->indexExist = true;
		return(true);
		// }
	}
	return (false);
}

bool Response::indexCgiCheck(void)
{
	std::string	tmp = this->request->getOldPath();

	// if (tmp[tmp.length() - 1] == '/')
	// 	tmp.erase(tmp.length() - 1);
	
	// if (tmp != this->request->location.getLocationName())
	// 	return false;
	
	// if (request->UriWithotLoc != "/")
	// 	return (false);
	if (request->newPath != request->location.getRoot())
	{
		return (false);
	}

	if (!this->request->location.getIndex().empty())
	{
		for (size_t i = 0; i < this->request->location.getIndex().size(); i++)
		{
			if (stat(this->request->location.getIndex().at(i).c_str(), &this->buf) == 0)
			{
				if (S_ISDIR(this->buf.st_mode))
				{
					this->indexExist = true;
					return false;
				}
				else
				{
					if (isThisPathExist(request->getNewPath(), this->request->location.getIndex().at(i)) == false)//! what is this
					{
						return (false);
					}
					return (isCGI(this->request->location.getIndex().at(i)));
				}
			}
		}
	}
	return false;
}

bool Response::cgi_Handler(const std::string& path)
{

	if (!this->preCGI)
	{
		char* args[3];
		this->status = 0;
		this->env = setCgiEnvironment(path);
		set_args(args, path);
		
		output_cgi = RandomName();
		// if (request->getMethdType() == POST)
		// 	input_cgi = this->uploadedFileName;
		startCgiTime = time(NULL);
		childToKill = true;
		this->pid = fork();

		if (this->pid == 0)
		{
			// if (request->getMethdType() == POST)
			// 	redirectCgiInput();
			redirectCgiOutput();
			if (chdir(getCgiFileRoot(path).c_str()) == -1)
				exit(EXIT_FAILURE);
			execve(args[0], args, this->env);
			delete[] this->env;
			delete[] args[0];
			delete[] args[1];
			exit(EXIT_FAILURE);
		}
		
		this->preCGI = true;
		delete[] this->env;
		delete[] args[0];
		delete[] args[1];
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
		return (true);// ! return true as well
	extractCgiMetadata();
	return (true);
}

// ************ Getters *********************
