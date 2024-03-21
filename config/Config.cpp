

#include "Config.hpp"

Config::Config(std::string const& filename)
{
	configFile.open(filename.c_str());
	if (!configFile)
		throw std::runtime_error("Failed to open config file");
}

Config::~Config(){configFile.close();}

void Config::pushBlock(std::string context)
{
	if (context == "server")
	{
		servers.push_back(server);
		server = ServerBlock();
	}
	else if (context == "location")
	{
		server.addLocation(location);
		location = LocationBlock();
	}
}

bool isBrace(std::string line)
{
	if (line.at(0) == '{' || line.at(0) == '}')
		return true;
	return false;
}

int Config::ckeckBraces(std::vector<std::string> line, std::string context)
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
			std::cout << "End of context: " << contexts.top() << std::endl;
			contexts.pop();
		}
		if (braces.size() != contexts.size())
			throw std::runtime_error("Config Error: check closing braces or balanced braces with no context #2");
		pushBlock(context);
		return 1;
	}
	return 0;
}

void Config::parseBlock(std::ifstream& configFile, std::string context)
{
	std::string line;
	std::vector<std::string> splitedLine;
	while (std::getline(configFile, line))
	{
		line = removeExtraSpaces(line);
		splitedLine = split(line, ' ');
		if (line.empty() || line[0] == '#')
			continue;
		if (splitedLine[0] == "{" && contexts.empty() == false)
			braces.push('{');
		else if (splitedLine[0] == "}" && braces.empty())
				throw std::runtime_error("Config Error: check closing braces or braces with no context #1");
		else if (splitedLine[0] == "}" && braces.empty() == false)
		{
			braces.pop();
			if (contexts.empty() == false)
			{
				std::cout << "End of context: " << contexts.top() << std::endl;
				contexts.pop();
			}
			if (braces.size() != contexts.size())
				throw std::runtime_error("Config Error: check closing braces or balanced braces with no context #2");
			pushBlock(context);
			return;
		}
		else if (splitedLine[0] == "server")
		{
			if (splitedLine.size() != 1 || contexts.empty() == false)
				throw std::runtime_error("Config Error: check server context");
			contexts.push(splitedLine[0]);
			parseBlock(configFile, "server");
		}
		else if (splitedLine[0] == "location")
		{
			if (contexts.empty())
				throw std::runtime_error("Config Error: location is outside server context");
			else if (contexts.top() != "server")
				throw std::runtime_error("Config Error: location is inside another location context");
			contexts.push(splitedLine[0]);
			location.parseLocationLine(splitedLine);
			parseBlock(configFile, "location");
		}
		else
		{
			if (contexts.size() != braces.size())
				throw std::runtime_error("Config Error: check braces #3");
			if (context == "server")
			{
				server.parseServerLine(splitedLine);
			}
			else if (context == "location")
			{
				location.parseLocationLine(splitedLine);
			}
		}
	}
	if (!std::getline(configFile, line) && (!braces.empty() || !contexts.empty()))
		throw std::runtime_error("Config Error: check braces #4");
}