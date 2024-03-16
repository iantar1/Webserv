


#ifndef BLOCK_HPP
# define BLOCK_HPP

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>

class Block
{
	private:
		typedef bool (Block::*parseFunctions)(std::string);
		std::map<std::string, parseFunctions> serverFields;
		std::map<std::string, parseFunctions> locationFields;
		// #######   start of block (server or location) fields  ########
		int listen;
		std::string host;
		std::vector<std::string> serverName;
		std::string root;
		std::vector<std::string> index;
		std::map<int, std::string> errorPage;
		bool autoIndex;
		std::vector<std::string> allowMethods;
		std::map<std::string, std::string> cgiPath;
		std::string locationName;
		std::vector<Block> location;
		int maxBodySize;
		// #######   end of block (server or location) fields  ########
		bool checkListen(std::string listen);
		bool checkHost(std::string host);
		bool checkServerName(std::string serverName);
		bool checkRoot(std::string root);
		bool checkIndex(std::string index);
		bool checkErrorPage(std::string errorPage);
		bool checkAutoIndex(std::string autoIndex);
		bool checkAllowMethods(std::string allowMethods);
		bool checkCgiPath(std::string cgiPath);
		bool checkLocation(std::string location);
		bool checkMaxBodySize(std::string maxBodySize);
	public:
		Block();
		~Block();
	
};

#endif