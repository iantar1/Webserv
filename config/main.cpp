

#include "ServerBlock.hpp"
#include "LocationBlock.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include "Config.hpp"

// class parse{
// 	private:
// 		ServerBlock server;
// 		LocationBlock location;
// 		std::stack<char> balance;
// 	public:
// 		std::ifstream configFile;
// 		std::vector<ServerBlock> servers;
// 		parse(std::string filename)
// 		{
// 			configFile.open(filename.c_str());
// 			if (!configFile)
// 				throw std::runtime_error("Failed to open config file");
// 		}
// 		~parse(){configFile.close();}
// 		void parseBlock(std::ifstream& configFile, std::string context)
// 		{
// 			std::string line;
// 			std::vector<std::string> splitedLine;
// 			while (std::getline(configFile, line))
// 			{
// 				line = removeExtraSpaces(line);
// 				splitedLine = split(line, ' ');
// 				if (line.empty() || line[0] == '#')
// 					continue;
// 				if (line == "{")
// 				{
// 					balance.push('{');
// 					continue;
// 				}
// 				else if (line == "}")
// 				{
// 					if (balance.empty()){
// 						std::cerr << "Error: extra closing brace" << std::endl;
// 						return;
// 					}
// 					if (context == "server")
// 					{
// 						servers.push_back(server);
// 						server = ServerBlock();
// 						balance.pop();
// 						return;
// 					}
// 					else if (context == "location")
// 					{
// 						server.addLocation(location);
// 						location = LocationBlock();
// 						balance.pop();
// 						return;
// 					}
// 				}
// 				else if (splitedLine[0] == "server")
// 				{
// 					parseBlock(configFile, "server");
// 				}
// 				else if (splitedLine[0] == "location")
// 				{
// 					location.parseLocationLine(line);
// 					parseBlock(configFile, "location");
// 				}
// 				else
// 				{
// 					if (context == "server")
// 					{
// 						server.parseServerLine(line);
// 					}
// 					else if (context == "location")
// 					{
// 						location.parseLocationLine(line);
// 					}
// 				}
// 			}
// 		}
		
// };


int main()
{
	Config p("config.conf");
	try
	{
		p.parseBlock(p.configFile, "server");
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	for (std::size_t i = 0; i < p.servers.size(); i++)
	{
		std::cout << ">>>>>>>>>>>  Server " << i << " <<<<<<<<<<<" << std::endl;
		std::cout << p.servers[i] << std::endl;
		// std::cout << p.servers[i].getLocations()[i].getLocationName() << std::endl;
	}
	// std::cout << p.servers[3].getHost()<< std::endl;
	return 0;
}

// map["/user/ibel-har"] = newLocation("/");
// map["/user"] = newLocation("/");
// map["/"] = newLocation("/");
// map["/"] = newLocation("/");

// server
// {
// 	fkjjlkjl
//  ljdfkjlkjdlkfj
//  }
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 