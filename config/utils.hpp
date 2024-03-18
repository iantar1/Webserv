


#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <cctype>


std::string removeExtraSpaces(std::string const& line);
std::vector<std::string> split(std::string line, char delim);
bool isPosNumber(std::string const& str);
int stringToPosInt(std::string const& str);
bool checkIp(std::string ip);
bool checkEnable(std::string enable);

// check if the

#endif
