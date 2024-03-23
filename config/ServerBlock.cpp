

#include "ServerBlock.hpp"
#include <set>
#include "utils.hpp"

ServerBlock::ServerBlock()
{
	initFieldsMap();
	initFields();
}

ServerBlock::~ServerBlock(){}

void ServerBlock::initFieldsMap()
{
	Fields["listen"] = &ServerBlock::checkListen;
	Fields["host"] = &ServerBlock::checkHost;
	Fields["server_name"] = &ServerBlock::checkServerName;
	Fields["root"] = &ServerBlock::checkRoot;
	Fields["index"] = &ServerBlock::checkIndex;
	Fields["error_page"] = &ServerBlock::checkErrorPage;
	Fields["max_body_size"] = &ServerBlock::checkMaxBodySize;
}

void ServerBlock::initFields()
{
	listen = -1;
	host = "";
	serverName.clear();
	root = "";
	indexes.clear();
	errorPages.clear();
	maxBodySize = 0;
	locations.clear();
}

void ServerBlock::parseServerLine(std::vector<std::string> line)
{
	if (Fields.find(line[0]) == Fields.end())
		throw std::runtime_error("Unknown field in server block: " + line[0]);
	bool result = (this->*Fields[line[0]])(line);
	if (!result)
		throw std::runtime_error("Invalid field in server block: " + line[0]);
}

bool ServerBlock::checkListen(std::vector<std::string> listen)
{
	int listenPort;
	if (listen.size() != 3 || *(listen.end() - 1) != ";")
		return false;
	if (!isPosNumber(listen[1]))
		return false;
	listenPort = stringToPosInt(listen[1]);
	if (listenPort < 0 || listenPort > 65535)
		return false;
	this->listen = listenPort;
	return true;
}

bool ServerBlock::checkHost(std::vector<std::string> host)
{
	std::string hostName;
	if (host.size() != 3 || *(host.end() - 1) != ";")
		return false;
	hostName = host[1];
	if (hostName.empty())
		return false;
	if (hostName != "localhost" && !checkIp(hostName))
		return false;
	this->host = hostName;
	return true;
}

bool ServerBlock::checkServerName(std::vector<std::string> serverName)
{
	std::string serverNameStr;
	if (serverName.size() < 3 || *(serverName.end() - 1) != ";")
		return false;
	for (std::size_t i = 1; i < serverName.size() - 1; i++)
	{
		if (serverName[i].empty())
			return false;
		this->serverName.push_back(serverName[i]);
	}
	return true;
}

bool ServerBlock::checkRoot(std::vector<std::string> root)
{
	std::string rootStr;
	if (root.size() != 3 || *(root.end() - 1) != ";")
		return false;
	rootStr = root[1];
	if (rootStr.empty())
		return false;
	this->root = rootStr;
	return true;
}

bool ServerBlock::checkIndex(std::vector<std::string> index)
{
	std::string indexStr;
	if (index.size() < 3 || *(index.end() - 1) != ";")
		return false;
	for (std::size_t i = 1; i < index.size() - 1; i++)
	{
		if (index[i].empty())
			return false;
		this->indexes.push_back(index[i]);
	}
	return true;
}

bool ServerBlock::checkErrorPage(std::vector<std::string> errorPage)
{
	std::string errorPageStr;
	int errorCode;
	if (errorPage.size() != 4 || *(errorPage.end() - 1) != ";")
		return false;
	if (!isPosNumber(errorPage[1]))
		return false;
	errorCode = stringToPosInt(errorPage[1]);
	if (errorCode < 100 || errorCode > 599)
		return false;
	errorPageStr = errorPage[2];
	if (errorPageStr.empty())
		return false;
	this->errorPages[errorCode] = errorPageStr;
	return true;
}

bool ServerBlock::checkMaxBodySize(std::vector<std::string> maxBodySize)
{
	int maxBodySizeInt;
	if (maxBodySize.size() != 3 || *(maxBodySize.end() - 1) != ";")
		return false;
	if (!isPosNumber(maxBodySize[1]))
		return false;
	maxBodySizeInt = stringToPosInt(maxBodySize[1]);
	if (maxBodySizeInt < 0)
		return false;
	this->maxBodySize = maxBodySizeInt;
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

LocationBlock  *ServerBlock::getLocation(std::string const& locationName) const
{
	if (locationName.empty())
		return NULL;
	for (std::size_t i = 0; i < this->locations.size(); i++)
	{
		if (this->locations[i].getLocationName() == locationName)
			return const_cast<LocationBlock*>(&this->locations[i]);
	}
	return NULL;
}

void ServerBlock::addLocation(LocationBlock const& location)
{
	this->locations.push_back(location);
}

void ServerBlock::checkServer(void)
{
	if (this->listen < 0 || this->host.empty() || this->root.empty())
		throw std::runtime_error("Error: required fields are missing in server block");
	if (this->locations.empty())
		throw std::runtime_error("Error: no location block found in server block");
}

bool ServerBlock::compareServerName(ServerBlock const& rhs) const
{
	std::vector<std::string> rhsServerName = rhs.serverName;
	if (this->serverName.empty() || rhsServerName.empty())
		return false;
	std::set<std::string> thisSet(this->serverName.begin(), this->serverName.end());
	std::set<std::string> rhsSet(rhsServerName.begin(), rhsServerName.end());
	std::size_t namesSize = thisSet.size() + rhsSet.size();
	std::set<std::string> serverNamesSet;
	serverNamesSet.insert(thisSet.begin(), thisSet.end());
	serverNamesSet.insert(rhsSet.begin(), rhsSet.end());
	if (serverNamesSet.size() != namesSize)
		return false;
	return true;
}

bool ServerBlock::operator==(ServerBlock const& rhs) const
{
	if (this->listen != rhs.listen || this->host != rhs.host || compareServerName(rhs))
		return false;
	return true;
}

std::ostream& operator<<(std::ostream& outstream, ServerBlock const& serverBlock)
{
	outstream << "listen: " << serverBlock.getListen() << std::endl;
	outstream << "host: " << serverBlock.getHost() << std::endl;
	outstream << "server_name: ";
	for (std::size_t i = 0; i < serverBlock.getServerName().size(); i++)
		outstream << serverBlock.getServerName()[i] << " ";
	outstream << std::endl;
	outstream << "root: " << serverBlock.getRoot() << std::endl;
	outstream << "indexes: ";
	for (std::size_t i = 0; i < serverBlock.getIndexes().size(); i++)
		outstream << serverBlock.getIndexes()[i] << " ";
	outstream << std::endl;
	outstream << "error_pages: ";
	for (std::map<int, std::string>::const_iterator it = serverBlock.getErrorPages().begin(); it != serverBlock.getErrorPages().end(); it++)
		outstream << it->first << " " << it->second << " ";
	outstream << std::endl;
	outstream << "max_body_size: " << serverBlock.getMaxBodySize() << std::endl;
	outstream << "locations: " << std::endl;
	std::vector<LocationBlock> locations = serverBlock.getLocations();
	for (std::size_t i = 0; i < locations.size(); i++)
	{
		std::cout << "###########  Location " << i << " ###########" << std::endl;
		outstream << locations[i] << std::endl;
	}
	return outstream;
}
