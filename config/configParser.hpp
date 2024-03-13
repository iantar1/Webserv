

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP


#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class configParser
{
	private:
		configParser();
		std::string filename;
		

	public:
		configParser(std::string filename);
		~configParser();
	
};


#endif
