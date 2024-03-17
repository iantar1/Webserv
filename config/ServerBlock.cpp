

#include "ServerBlock.hpp"


ServerBlock::ServerBlock()
{
	initFields();
}

ServerBlock::~ServerBlock()
{
}

void ServerBlock::initFields()
{
	Fields["listen"] = &ServerBlock::checkListen;
	Fields["host"] = &ServerBlock::checkHost;
	Fields["server_name"] = &ServerBlock::checkServerName;
	Fields["root"] = &ServerBlock::checkRoot;
	Fields["index"] = &ServerBlock::checkIndex;
	Fields["error_page"] = &ServerBlock::checkErrorPage;
	Fields["max_body_size"] = &ServerBlock::checkMaxBodySize;
}

bool ServerBlock::parseServerLine(std::string line)
{
	std::size_t found = line.find(" ");
	if (found == std::string::npos)
		return false;
	std::string field = line.substr(0, found);
	std::string value = line.substr(found + 1);
	if (Fields.find(field) == Fields.end())
		return false;
	return (this->*Fields[field])(value);
}

bool ServerBlock::checkListen(std::string listen)
{
	listen = getvalue(listen);
	if (listen.empty())
		return false;
	this->listen = std::stoi(listen);
	return true;
}

bool ServerBlock::checkHost(std::string host)
{
	host = getvalue(host);
	if (host.empty())
		return false;
	this->host = host;
	return true;
}

bool ServerBlock::checkServerName(std::string serverName)
{
	serverName = getvalue(serverName);
	if (serverName.empty())
		return false;
	std::istringstream iss(serverName);
	std::string temp;
	while (iss >> temp)
		this->serverName.push_back(temp);
	return true;
}

bool ServerBlock::checkRoot(std::string root)
{
	root = getvalue(root);
	if (root.empty())
		return false;
	this->root = root;
	return true;
}

bool ServerBlock::checkIndex(std::string index)
{
	index = getvalue(index);
	if (index.empty())
		return false;
	std::istringstream iss(index);
	std::string temp;
	while (iss >> temp)
		this->indexes.push_back(temp);
	return true;
}

bool ServerBlock::checkErrorPage(std::string errorPage)
{
	errorPage = getvalue(errorPage);
	if (errorPage.empty())
		return false;
	std::size_t found = errorPage.find(" ");
	if (found == std::string::npos)
		return false;
	std::string code = errorPage.substr(0, found);
	std::string page = errorPage.substr(found + 1);
	if (code.empty() || page.empty())
		return false;
	this->errorPages[std::stoi(code)] = page;
	return true;
}

bool ServerBlock::checkMaxBodySize(std::string maxBodySize)
{
	maxBodySize = getvalue(maxBodySize);
	if (maxBodySize.empty())
		return false;
	this->maxBodySize = std::stoi(maxBodySize);
	return true;
}

bool ServerBlock::checkServer(void)
{
	if (this->listen == 0 || this->host.empty() || this->serverName.empty() || this->root.empty() || this->indexes.empty() || this->errorPages.empty() || this->maxBodySize == 0)
		return false;
	return true;
}

int const& ServerBlock::getListen() const
{
	return this->listen;
}

std::string const& ServerBlock::getHost() const
{
	return this->host;
}

std::vector<std::string> const& ServerBlock::getServerName() const
{
	return this->serverName;
}

std::string const& ServerBlock::getRoot() const
{
	return this->root;
}

std::vector<std::string> const& ServerBlock::getIndexes() const
{
	return this->indexes;
}

std::map<int, std::string> const& ServerBlock::getErrorPages() const
{
	return this->errorPages;
}

int const& ServerBlock::getMaxBodySize() const
{
	return this->maxBodySize;
}

std::vector<LocationBlock> const& ServerBlock::getLocations() const
{
	return this->locations;
}

void ServerBlock::addLocation(LocationBlock const& location)
{
	this->locations.push_back(location);
}


