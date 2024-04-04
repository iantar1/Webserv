

#include "../includes/Response.hpp"

bool	Response::readyToPost()
{
	if (request->getDoneHeaderReading() == false)
		return (false);
	if (request->getError() != 0)
		return (false);
	return (true);
}

void	Response::post()
{
	if (!readyToPost())
		return ;
	openPostOutFile();
	if (request->getTransferMode() == CHUNKED_POST)
		chunkedPost();
	else
	{
		contentLenght = request->getContentLength();
		std::cerr << RED <<"contentLenght: " << contentLenght << RESET << std::endl;
		normalPost();
	}
	// request->setDoneServing();
}


void Response::chunkedPost(void)
{
	std::cerr << "chunked post is not implemented yet" << std::endl;
}

void Response::normalPost(void)
{
	std::string body = request->getBody();
	if (body.empty())
		return ;
	postOutFile.write(body.c_str(), body.size());
	std::cout << "body: " << body << std::endl;
	std::cout << "content-lenght: " << contentLenght << std::endl;
	postedSize += body.size();
	if (postedSize > contentLenght)
	{
		std::cerr << "posted size is greater than content lenght" << std::endl;
		// unlink(postOutFileName.c_str());
	}
}

std::string Response::contentTypeExt() const
{
	std::map<std::string, std::string>::const_iterator it;
	std::map<std::string, std::string>::const_iterator it2;
	std::string empty = "";
	it = request->getHeaders().find("content-type");
	if (it == request->getHeaders().end())
		return empty;
	it2 = files.mime.find(it->second);
	if (it2 == files.mime.end())
		return empty;
	return it2->second;
}

// ! ckeck if there is no content type int request

void Response::openPostOutFile(void)
{
	// ! check if the upload is enabled
	if (request->getMethdType() != POST)
		return ;
	std::string uploadDir = request->location.getUploadPath();
	postOutFileName = generateFileName(contentTypeExt());
	postOutFileName += getFileExtension(postOutFileName);
	postOutFileName = uploadDir + "/" + postOutFileName;
	std::cerr << RED << "postOutFileName: " << postOutFileName << RESET << std::endl;
	postOutFile.open(postOutFileName.c_str(), std::ios::binary);
}


