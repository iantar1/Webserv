


#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>

#include "LocationBlock.hpp"

class ServerBlock
{
	private:
		typedef bool (ServerBlock::*parseFunctions)(std::vector<std::string>);
		std::map<std::string, parseFunctions> Fields;
		// #######   start of ServerBlock   ########
		int listen;
		std::string host;
		std::vector<std::string> serverName;
		std::string root;
		std::vector<std::string> indexes;
		std::map<int, std::string> errorPages;
		int maxBodySize;
		std::vector<LocationBlock> locations;
		// #######   end of ServerBlock   ########
		bool checkListen(std::vector<std::string> listen);
		bool checkHost(std::vector<std::string> host);
		bool checkServerName(std::vector<std::string> serverName);
		bool checkRoot(std::vector<std::string> root);
		bool checkIndex(std::vector<std::string> index);
		bool checkErrorPage(std::vector<std::string> errorPage);
		bool checkMaxBodySize(std::vector<std::string> maxBodySize);
	public:
		ServerBlock();
		~ServerBlock();
		// getters
		int const& getListen() const;
		std::string const& getHost() const;
		std::vector<std::string> const& getServerName() const;
		std::string const& getRoot() const;
		std::vector<std::string> const& getIndexes() const;
		std::map<int, std::string> const& getErrorPages() const;
		int const& getMaxBodySize() const;
		std::vector<LocationBlock> const& getLocations() const;
		// end of getters
		void addLocation(LocationBlock const& location);
		void initFieldsMap();
		void initFields();
		bool parseServerLine(std::vector<std::string> line);
		bool checkServer(void);
	
};

std::ostream& operator<<(std::ostream& outstream, ServerBlock const& serverBlock);

#endif