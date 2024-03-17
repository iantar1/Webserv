


#include "LocationBlock.hpp"


LocationBlock::LocationBlock()
{
	initFields();
}

LocationBlock::~LocationBlock()
{
}

void LocationBlock::initFields()
{
	Fields["location"] = &LocationBlock::checkLocation;
	Fields["root"] = &LocationBlock::checkRoot;
	Fields["index"] = &LocationBlock::checkIndex;
	Fields["autoindex"] = &LocationBlock::checkAutoIndex;
	Fields["allow_methods"] = &LocationBlock::checkAllowMethods;
	Fields["cgi_path"] = &LocationBlock::checkCgiPath;
	Fields["upload_path"] = &LocationBlock::checkUploadPath;
	Fields["upload_enable"] = &LocationBlock::checkUploadEnable;
	Fields["cgi_enable"] = &LocationBlock::checkCgiEnable;
}

bool LocationBlock::parseLocationLine(std::string line)
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

bool LocationBlock::checkLocation(std::string location)
{
	location = getvalue(location);
	if (location.empty())
		return false;
	this->locationName = location;
	return true;
}

bool LocationBlock::checkRoot(std::string root)
{
	root = getvalue(root);
	if (root.empty())
		return false;
	this->root = root;
	return true;
}

bool LocationBlock::checkIndex(std::string index)
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

bool LocationBlock::checkAutoIndex(std::string autoIndex)
{
	autoIndex = getvalue(autoIndex);
	if (autoIndex.empty())
		return false;
	if (autoIndex == "on")
		this->autoIndex = true;
	else if (autoIndex == "off")
		this->autoIndex = false;
	else
		return false;
	return true;
}

bool LocationBlock::checkAllowMethods(std::string allowMethods)
{
	allowMethods = getvalue(allowMethods);
	if (allowMethods.empty())
		return false;
	std::istringstream iss(allowMethods);
	std::string temp;
	while (iss >> temp)
		this->allowMethods.push_back(temp);
	return true;
}

bool LocationBlock::checkCgiPath(std::string cgiPath)
{
	cgiPath = getvalue(cgiPath);
	if (cgiPath.empty())
		return false;
	std::size_t found = cgiPath.find(" ");
	if (found == std::string::npos)
		return false;
	std::string method = cgiPath.substr(0, found);
	std::string path = cgiPath.substr(found + 1);
	this->cgiPaths[method] = path;
	return true;
}

bool LocationBlock::checkUploadPath(std::string uploadPath)
{
	uploadPath = getvalue(uploadPath);
	if (uploadPath.empty())
		return false;
	this->uploadPath = uploadPath;
	return true;
}

bool LocationBlock::checkUploadEnable(std::string uploadEnable)
{
	uploadEnable = getvalue(uploadEnable);
	if (uploadEnable.empty())
		return false;
	if (uploadEnable == "on")
		this->uploadEnable = true;
	else if (uploadEnable == "off")
		this->uploadEnable = false;
	else
		return false;
	return true;
}

bool LocationBlock::checkCgiEnable(std::string cgiEnable)
{
	cgiEnable = getvalue(cgiEnable);
	if (cgiEnable.empty())
		return false;
	if (cgiEnable == "on")
		this->cgiEnable = true;
	else if (cgiEnable == "off")
		this->cgiEnable = false;
	else
		return false;
	return true;
}

std::string const& LocationBlock::getLocationName() const
{
	return this->locationName;
}

std::string const& LocationBlock::getRoot() const
{
	return this->root;
}

std::vector<std::string> const& LocationBlock::getIndex() const
{
	return this->indexes;
}

bool const& LocationBlock::getAutoIndex() const
{
	return this->autoIndex;
}

std::vector<std::string> const& LocationBlock::getAllowMethods() const
{
	return this->allowMethods;
}

std::map<std::string, std::string> const& LocationBlock::getCgiPaths() const
{
	return this->cgiPaths;
}

std::string const& LocationBlock::getUploadPath() const
{
	return this->uploadPath;
}

bool const& LocationBlock::getUploadEnable() const
{
	return this->uploadEnable;
}

bool const& LocationBlock::getCgiEnable() const
{
	return this->cgiEnable;
}



