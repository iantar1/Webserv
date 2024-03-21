


#ifndef CONFIG_HPP
# define CONFIG_HPP


#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include "ServerBlock.hpp"
#include "LocationBlock.hpp"
#include "utils.hpp"

class Config
{
	private:
		ServerBlock server;
		LocationBlock location;
		std::stack<char> braces;
		std::stack<std::string> contexts;
		void pushBlock(std::string context);	
	public:
		std::ifstream configFile;
		std::vector<ServerBlock> servers; 
		Config(std::string const& filename);
		~Config();
		void parseBlock(std::ifstream& configFile, std::string context);
};


#endif