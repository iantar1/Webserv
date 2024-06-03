


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
		int										fdSocket;
		int										sid;
		std::string								listen;
		std::string								host;
		std::vector<std::string>				serverName;
		std::string								root;
		std::vector<std::string>				indexes;
		std::map<int, std::string>				errorPages;
		int										maxBodySize;
		std::map<std::string, LocationBlock>	locations;
 
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
		int											getFdSocket() const;
		int											getSid() const;
		std::string const&							getListen() const;
		std::string const&							getHost() const;
		std::vector<std::string> const&				getServerName() const;
		std::string const& 							getRoot() const;
		std::vector<std::string> const&				getIndexes() const;
		std::map<int, std::string> const&			getErrorPages() const;
		int const& 									getMaxBodySize() const;
		std::map<std::string, LocationBlock> const&	getLocations() const;
		LocationBlock								*getLocation(std::string const&	locationName) const;

		void setFdSocket(int);
		void setSid(int);

		bool compareServerName(ServerBlock const& rhs) const;
	// operator == overload
		bool operator==(ServerBlock const& rhs) const;
		void addLocation(LocationBlock const& location);
		void initFieldsMap();
		void initFields();
		void parseServerLine(std::vector<std::string> line);
		void checkServer(void);
		bool findHostName(std::string const& hsotName) const;
	
};

std::ostream& operator<<(std::ostream& outstream, ServerBlock const& serverBlock);

#endif

