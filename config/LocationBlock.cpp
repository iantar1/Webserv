


#include "LocationBlock.hpp"
#include "utils.hpp"

LocationBlock::LocationBlock()
{
	initFieldsMap();
	initFields();
}

LocationBlock::~LocationBlock()
{
}

void LocationBlock::initFieldsMap()
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

void LocationBlock::initFields()
{
	locationName = "";
	root = "";
	indexes.clear();
	autoIndex = false;
	allowMethods.clear();
	cgiPaths.clear();
	uploadPath = "";
	uploadEnable = false;
	cgiEnable = false;
}

void LocationBlock::parseLocationLine(std::vector<std::string> line)
{
	if (Fields.find(line[0]) == Fields.end())
		throw std::runtime_error("Unknown field in location block: " + line[0]);
	bool result = (this->*Fields[line[0]])(line);
	if (!result)
		throw std::runtime_error("Invalid field in location block: " + line[0]);
}

bool LocationBlock::checkLocation(std::vector<std::string> location)
{
	std::string locationName;
	if (location.size() != 2)
		return false;
	locationName = location[1];
	if (locationName.empty())
		return false;
	this->locationName = locationName;
	return true;	
}

bool LocationBlock::checkRoot(std::vector<std::string> root)
{
	std::string rootPath;
	if (root.size() != 3 || *(root.end() - 1) != ";")
		return false;
	rootPath = root[1];
	if (rootPath.empty())
		return false;
	this->root = rootPath;
	return true;
}

bool LocationBlock::checkIndex(std::vector<std::string> index)
{
	std::vector<std::string> indexes;
	if (index.size() < 3 || *(index.end() - 1) != ";")
		return false;
	for (std::size_t i = 1; i < index.size() - 1; i++)
	{
		if (index[i].empty())
			return false;
		indexes.push_back(index[i]);
	}
	this->indexes = indexes;
	return true;
}

bool LocationBlock::checkAutoIndex(std::vector<std::string> autoIndex)
{
	bool autoIndexBool;
	if (autoIndex.size() != 3 || *(autoIndex.end() - 1) != ";")
		return false;
	if (autoIndex[1] == "on")
		autoIndexBool = true;
	else if (autoIndex[1] == "off")
		autoIndexBool = false;
	else
		return false;
	this->autoIndex = autoIndexBool;
	return true;
}

bool LocationBlock::checkAllowMethods(std::vector<std::string> allowMethods)
{
	std::vector<std::string> methods;
	std::vector<std::string> allowedMethods;
	methods.push_back("GET");
	methods.push_back("POST");
	methods.push_back("DELETE");
	if (allowMethods.size() < 3 || *(allowMethods.end() - 1) != ";")
		return false;
	for (std::size_t i = 1; i < allowMethods.size() - 1; i++)
	{
		if (std::find(methods.begin(), methods.end(), allowMethods[i]) == methods.end())
			return false;
		allowedMethods.push_back(allowMethods[i]);
	}
	this->allowMethods = allowedMethods;
	return true;
}

bool LocationBlock::checkCgiPath(std::vector<std::string> cgiPath)
{
	std::string cgiPathStr;
	std::string cgiExt;
	if (cgiPath.size() != 4 || *(cgiPath.end() - 1) != ";")
		return false;
	cgiExt = cgiPath[1];
	cgiPathStr = cgiPath[2];
	if (cgiExt.empty() || cgiPathStr.empty())
		return false;
	this->cgiPaths[cgiExt] = cgiPathStr;
	return true;
}

bool LocationBlock::checkUploadPath(std::vector<std::string> uploadPath)
{
	bool uploadPathBool;
	if (uploadPath.size() != 3 || *(uploadPath.end() - 1) != ";")
		return false;
	if (uploadPath[1] == "on")
		uploadPathBool = true;
	else if (uploadPath[1] == "off")
		uploadPathBool = false;
	else
		return false;
	this->uploadEnable = uploadPathBool;
	return true;
}

bool LocationBlock::checkUploadEnable(std::vector<std::string> uploadEnable)
{
	bool uploadEnableBool;
	if (uploadEnable.size() != 3 || *(uploadEnable.end() - 1) != ";")
		return false;
	if (uploadEnable[1] == "on")
		uploadEnableBool = true;
	else if (uploadEnable[1] == "off")
		uploadEnableBool = false;
	else
		return false;
	this->uploadEnable = uploadEnableBool;
	return true;
}

bool LocationBlock::checkCgiEnable(std::vector<std::string> cgiEnable)
{
	bool cgiEnableBool;
	if (cgiEnable.size() != 3 || *(cgiEnable.end() - 1) != ";")
		return false;
	if (cgiEnable[1] == "on")
		cgiEnableBool = true;
	else if (cgiEnable[1] == "off")
		cgiEnableBool = false;
	else
		return false;
	this->cgiEnable = cgiEnableBool;
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

std::ostream& operator<<(std::ostream& outstream, LocationBlock const& locationBlock)
{
	outstream << "location: " << locationBlock.getLocationName() << std::endl;
	outstream << "root: " << locationBlock.getRoot() << std::endl;
	outstream << "index: ";
	for (std::size_t i = 0; i < locationBlock.getIndex().size(); i++)
		outstream << locationBlock.getIndex()[i] << " ";
	outstream << std::endl;
	outstream << "autoindex: " << locationBlock.getAutoIndex() << std::endl;
	outstream << "allow_methods: ";
	for (std::size_t i = 0; i < locationBlock.getAllowMethods().size(); i++)
		outstream << locationBlock.getAllowMethods()[i] << " ";
	outstream << std::endl;
	outstream << "cgi_paths: ";
	for (std::map<std::string, std::string>::const_iterator it = locationBlock.getCgiPaths().begin(); it != locationBlock.getCgiPaths().end(); it++)
		outstream << it->first << " " << it->second << " ";
	outstream << std::endl;
	outstream << "upload_path: " << locationBlock.getUploadPath() << std::endl;
	outstream << "upload_enable: " << locationBlock.getUploadEnable() << std::endl;
	outstream << "cgi_enable: " << locationBlock.getCgiEnable() << std::endl;
	return outstream;
}

