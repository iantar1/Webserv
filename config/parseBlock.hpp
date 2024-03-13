


#ifndef PARSEBLOCK_HPP
#define PARSEBLOCK_HPP

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>

class parseBlock
{
	typedef bool (parseBlock::*parseFunctions)(std::string);
	typedef std::map<std::string, parseFunctions> serverFields;
	typedef std::map<std::string, parseFunctions> locationFields;
	private:
		
	public:
		parseBlock();
		~parseBlock();
	
};

#endif