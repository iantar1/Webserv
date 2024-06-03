


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
		ServerBlock 			server;
		LocationBlock 			location;
		std::stack<char> 		braces;
		std::stack<std::string>	contexts;
		std::vector<ServerBlock> tmpServers;
		int id;

	// * private Methods *
		void	pushBlock(std::string context);
		void	parseContextLine(std::vector<std::string> line, std::string context);
		void	addServerToContexts(std::vector<std::string> line);
		void	addLocationToContexts(std::vector<std::string> line);
		bool	checkBraces(std::vector<std::string> line, std::string context);
		void	insertLocationRoot(void);
	public:
		std::ifstream configFile;
		std::vector<ServerBlock> servers;
		std::map<int, std::vector<ServerBlock> > similars;
		Config(std::string const& filename);
		~Config();
		void parseBlock(std::ifstream& configFile, std::string context);
		void parse(void);
		bool checkDupServers();
};


#endif