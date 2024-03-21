


// int Config::bracketsBalance(std::vector<std::string> line , std::string context)
// {
// 	if (line[0] == "{" && contexts.empty() == false)
// 			braces.push('{');
// 	else if (line[0] == "}" && braces.empty())
// 			throw std::runtime_error("Config Error: check closing braces or braces with not context");
// 	else if (line[0] == "}" && braces.empty() == false)
// 	{
// 		if (!braces.empty() || !contexts.empty())
// 			throw std::runtime_error("Config Error: check closing braces or balanced braces with no context");
// 		braces.pop();
// 		if (contexts.empty() == false)
// 		{
// 			std::cout << "End of context: " << contexts.top() << std::endl;
// 			contexts.pop();
// 			return 1;
// 		}
// 	}
// 	else if (line[0] == "server" && contexts.empty() == false)
// 		throw std::runtime_error("Config Error: check server context");
// 	else if (line[0] == "location" && contexts.empty())
// 		throw std::runtime_error("Config Error: location is outside server context");
// 	else if (line[0] == "location" && contexts.top() != "server")
// 		throw std::runtime_error("Config Error: location is inside another location context");
// 	else
// 		contexts.push(line[0]);
// 	return 0;
// }



// void Config::parseBlock(std::ifstream& configFile, std::string context)
// {
// 	std::string line;
// 	std::vector<std::string> splitedLine;
// 	while (std::getline(configFile, line))
// 	{
// 		line = removeExtraSpaces(line);
// 		splitedLine = split(line, ' ');
// 		if (line.empty() || line[0] == '#')
// 			continue;
// 		if (line == "{")
// 		{
// 			braces.push('{');
// 			continue;
// 		}
// 		else if (line == "}")
// 		{
// 			if (braces.empty()){
// 				std::cerr << "Error: extra closing brace" << std::endl;
// 				return;
// 			}
// 			if (context == "server")
// 			{
// 				servers.push_back(server);
// 				server = ServerBlock();
// 				braces.pop();
// 				return;
// 			}
// 			else if (context == "location")
// 			{
// 				server.addLocation(location);
// 				location = LocationBlock();
// 				braces.pop();
// 				return;
// 			}
// 		}
// 		else if (splitedLine[0] == "server")
// 		{
// 			parseBlock(configFile, "server");
// 		}
// 		else if (splitedLine[0] == "location")
// 		{
// 			location.parseLocationLine(splitedLine);
// 			parseBlock(configFile, "location");
// 		}
// 		else
// 		{
// 			if (context == "server")
// 			{
// 				server.parseServerLine(splitedLine);
// 			}
// 			else if (context == "location")
// 			{
// 				location.parseLocationLine(splitedLine);
// 			}
// 		}
// 	}
// }



// int Config::bracketsBalance(std::vector<std::string> line , std::string context)
// {
// 	(void)context;
// 	if (line[0] == "{" && contexts.empty() == false)
// 			braces.push('{');
// 	else if (line[0] == "}" && braces.empty())
// 			throw std::runtime_error("Config Error: check closing braces or braces with no context #1");
// 	else if (line[0] == "}" && braces.empty() == false)
// 	{
// 		braces.pop();
// 		if (contexts.empty() == false)
// 		{
// 			std::cout << "End of context: " << contexts.top() << std::endl;
// 			contexts.pop();
// 		}
// 		if (braces.size() != contexts.size())
// 			throw std::runtime_error("Config Error: check closing braces or balanced braces with no context #2");
// 		return 1;
// 	}
// 	return 0;
// }

// void Config::checkContextNesting(std::vector<std::string> line)
// {
// 	std::string fileLine;
// 	for (std::size_t i = 0; i < line.size(); i++)
// 		fileLine += line[i] + " ";
// 	std::cout << "Context: " << fileLine << std::endl;
// 	if (line[0] == "server" && (line.size() != 1 ||  contexts.empty() == false))
// 		throw std::runtime_error("Config Error: check server context");
// 	else if (line[0] == "location" && contexts.empty())
// 		throw std::runtime_error("Config Error: location is outside server context");
// 	else if (line[0] == "location" && contexts.top() != "server")
// 		throw std::runtime_error("Config Error: location is inside another location context");
// 	else if (line[0] == "location")
// 		location.parseLocationLine(line);
// 	contexts.push(line[0]);
// }

// void Config::parseBlock(std::ifstream& configFile, std::string context)
// {
// 	std::string line;
// 	std::vector<std::string> splitedLine;
// 	while (std::getline(configFile, line))
// 	{
// 		line = removeExtraSpaces(line);
// 		splitedLine = split(line, ' ');
// 		if (line.empty() || line[0] == '#')
// 			continue;
// 		if ((splitedLine[0] == "{" || splitedLine[0] == "}"))
// 		{
// 			if (bracketsBalance(splitedLine, context) == 1)
// 			{
// 				pushBlock(contexts.top());
// 				return;
// 			}
// 				continue;
// 		}
// 		else if (splitedLine[0] == "server")
// 		{
// 			if (contexts.empty() == false)
// 				throw std::runtime_error("Config Error: check server context");
// 			contexts.push("server");
// 			parseBlock(configFile, "server");
// 		}
// 		else if (splitedLine[0] == "location")
// 		{
// 			if (contexts.empty())
// 				throw std::runtime_error("Config Error: location is outside server context");
// 			else if (contexts.top() != "server")
// 				throw std::runtime_error("Config Error: location is inside another location context");
// 			contexts.push("location");
// 			location.parseLocationLine(splitedLine);
// 			parseBlock(configFile, splitedLine[0]);
// 		}
// 		else if (context == "server")
// 			server.parseServerLine(splitedLine);
// 		else if (context == "location")
// 			location.parseLocationLine(splitedLine);
// 	}
// 	if (!std::getline(configFile, line) && !braces.empty() && !contexts.empty())
// 		throw std::runtime_error("Config Error: check braces #4");
// }


// ************* last version *************

// void Config::parseBlock(std::ifstream& configFile, std::string context)
// {
// 	std::string line;
// 	std::vector<std::string> splitedLine;
// 	while (std::getline(configFile, line))
// 	{
// 		line = removeExtraSpaces(line);
// 		splitedLine = split(line, ' ');
// 		if (line.empty() || line[0] == '#')
// 			continue;
// 		if (splitedLine[0] == "{" && contexts.empty() == false)
// 			braces.push('{');
// 		else if (splitedLine[0] == "}" && braces.empty())
// 				throw std::runtime_error("Config Error: check closing braces or braces with no context #1");
// 		else if (splitedLine[0] == "}" && braces.empty() == false)
// 		{
// 			braces.pop();
// 			if (contexts.empty() == false)
// 			{
// 				std::cout << "End of context: " << contexts.top() << std::endl;
// 				contexts.pop();
// 			}
// 			if (braces.size() != contexts.size())
// 				throw std::runtime_error("Config Error: check closing braces or balanced braces with no context #2");
// 			pushBlock(context);
// 			return;
// 		}
// 		else if (splitedLine[0] == "server")
// 		{
// 			if (splitedLine.size() != 1 || contexts.empty() == false)
// 				throw std::runtime_error("Config Error: check server context");
// 			contexts.push(splitedLine[0]);
// 			parseBlock(configFile, "server");
// 		}
// 		else if (splitedLine[0] == "location")
// 		{
// 			if (contexts.empty())
// 				throw std::runtime_error("Config Error: location is outside server context");
// 			else if (contexts.top() != "server")
// 				throw std::runtime_error("Config Error: location is inside another location context");
// 			contexts.push(splitedLine[0]);
// 			location.parseLocationLine(splitedLine);
// 			parseBlock(configFile, "location");
// 		}
// 		else
// 		{
// 			if (contexts.size() != braces.size())
// 				throw std::runtime_error("Config Error: check braces #3");
// 			if (context == "server")
// 			{
// 				server.parseServerLine(splitedLine);
// 			}
// 			else if (context == "location")
// 			{
// 				location.parseLocationLine(splitedLine);
// 			}
// 		}
// 	}
// 	if (!std::getline(configFile, line) && (!braces.empty() || !contexts.empty()))
// 		throw std::runtime_error("Config Error: check braces #4");
// }
