

#include "../includes/Config.hpp"
#include <set>

Config::Config(std::string const &filename)
{
	configFile.open(filename.c_str());
	if (!configFile)
		throw std::runtime_error("Failed to open config file");
}

Config::~Config()
{
	configFile.close();
}

void Config::insertLocationRoot(void)
{
	std::string root = "root " + server.getRoot() + " ;";
	std::vector<std::string> rootLine = split(root, ' ');
	if (location.getRoot().empty())
		location.parseLocationLine(rootLine);
}

bool Config::checkDupServers()
{
	bool duplicate = false;
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (server == servers[i] && !server.compareServerName(servers[i]))
			throw std::runtime_error("Error: duplicate server block");
		else if (server == servers[i] && server.compareServerName(servers[i]))
			duplicate = true;
	}
	return duplicate;
}

void Config::pushBlock(std::string context)
{
	if (context == "server")
	{
		server.checkServer();
		if (!checkDupServers())
			servers.push_back(server);
		server = ServerBlock();
	}
	else if (context == "location")
	{
		insertLocationRoot();
		server.addLocation(location);
		location = LocationBlock();
	}
}

void Config::parseContextLine(std::vector<std::string> line, std::string context)
{
	if (contexts.size() != braces.size())
		throw std::runtime_error("Config Error: check braces #3");
	if (context == "location")
		location.parseLocationLine(line);
	else if (context == "server" && server.getLocations().empty())
	{
		server.parseServerLine(line);
	}
	else
		throw std::runtime_error("Config Error: server directives must be before location blocks");
}

void Config::addServerToContexts(std::vector<std::string> line)
{
	if (line.size() != 1)
		throw std::runtime_error("Config Error: check server context");
	if (contexts.empty() == false)
		throw std::runtime_error("Config Error: server is inside another context");
	contexts.push(line[0]);
}

void Config::addLocationToContexts(std::vector<std::string> line)
{
	if (contexts.empty() || contexts.top() != "server")
		throw std::runtime_error("Config Error: location must be inside server context");
	contexts.push(line[0]);
}
// check signle braces : {{}}
bool Config::checkBraces(std::vector<std::string> line, std::string context)
{
	if ((line[0] == "{" || line[0] == "}") && line.size() != 1)
		throw std::runtime_error("Config Error: check braces #1");
	if (line[0] == "{" && contexts.empty() == false)
		braces.push('{');
	else if (line[0] == "}" && braces.empty())
		throw std::runtime_error("Config Error: check closing braces or braces with no context #1");
	else if (line[0] == "}" && braces.empty() == false)
	{
		braces.pop();
		if (contexts.empty() == false)
		{
			// std::cout << "End of context: " << contexts.top() << std::endl;
			contexts.pop();
		}
		if (braces.size() != contexts.size())
			throw std::runtime_error("Config Error: check closing braces or balanced braces with no context #2");
		pushBlock(context);
		return 1;
	}
	return 0;
}

void Config::parseBlock(std::ifstream &configFile, std::string context)
{
	std::string line;
	std::vector<std::string> splitedLine;
	bool endOfBlock = false;
	while (std::getline(configFile, line))
	{
		line = removeExtraSpaces(line);
		// std::cout << "Line: " << line << std::endl;
		splitedLine = split(line, ' ');
		if (line.empty() || line[0] == '#')
			continue;
		// std::cout << "Splited Line: " << splitedLine[0] << std::endl;
		endOfBlock = checkBraces(splitedLine, context);
		if (isBrace(line) && !endOfBlock)
			continue;
		else if (isBrace(line) && endOfBlock)
			return;
		else if (splitedLine[0] == "server")
		{
			addServerToContexts(splitedLine);
			parseBlock(configFile, "server");
		}
		else if (splitedLine[0] == "location")
		{
			addLocationToContexts(splitedLine);
			location.parseLocationLine(splitedLine);
			parseBlock(configFile, "location");
		}
		else
			parseContextLine(splitedLine, context);
	}
	if (!std::getline(configFile, line) && (!braces.empty() || !contexts.empty()))
		throw std::runtime_error("Config Error: check braces #4");
}

void Config::parse(void)
{
	parseBlock(configFile, "server");
	if (servers.empty())
		throw std::runtime_error("Config Error: no server blocks found");
	configFile.close();
}