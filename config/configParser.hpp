

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP


#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class ConfigParser
{
	private:
		ConfigParser();
		std::string filename;
		

	public:
		ConfigParser(std::string filename);
		
		~ConfigParser();
	
};


#endif
