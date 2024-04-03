

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <vector>
#include <algorithm>
#include <cctype>
#include <set>
#include <string>
#define SPACE 32

std::string removeExtraSpaces(std::string const& line)
{
	std::string newStr;
	std::istringstream iss(line);
	std::string temp;
	
	while (iss >> temp)
		newStr += temp + " ";
	if (newStr[newStr.length() - 1] == ' ')
		newStr.erase(--newStr.end());
	std::size_t i = newStr.rfind(';');
	if (i != std::string::npos && newStr[i - 1] != ' ')
		newStr.insert(i, " ");
	return newStr;
}

std::vector<std::string> split(std::string line, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(line);
	std::string temp;
	while (std::getline(iss, temp, delim))
	{
		result.push_back(temp);
	}
	return result;
}

std::map<std::string, std::string> mimeTypes(void)
{
	std::map<std::string, std::string> out;
	std::ifstream inFile("../DataBase/MIME.type");
	std::string line;
	std::vector<std::string> splitLine;
	int i;

	if (!inFile.is_open()) {
		std::cerr << "Couldn't open MIME.type file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (std::getline(inFile, line))
	{
		line = removeExtraSpaces(line);
		splitLine = split(line, SPACE);
		out.insert(std::make_pair(splitLine[0], splitLine[1]));
	}
	inFile.close();
	return out;
}

void print_map(std::map<std::string, std::string> &out)
{
	std::map<std::string, std::string>::iterator it = out.begin();
	while (it != out.end())
	{
		std::cout << it->first << " : " << it->second << std::endl;
		it++;
	}
}

int main(void)
{
	std::map<std::string, std::string> out;
	out = mimeTypes();
	print_map(out);
}