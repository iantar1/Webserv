

#include "utiles.hpp"

std::string& removeExtraSpaces(std::string const& line)
{
	std::string newStr;
	std::istringstream iss(line);
	std::string temp;
	
	while (iss >> temp)
		newStr += temp + " ";
	if (newStr[newStr.length() - 1] == ' ')
		newStr.erase(--newStr.end());
	return newStr;
}

std::vector<std::string>& split(std::string line, char delim)
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