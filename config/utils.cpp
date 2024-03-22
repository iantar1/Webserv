

#include "utils.hpp"

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

bool isPosNumber(std::string const& str)
{
	for (std::size_t i = 0 ; i < str.length(); i++)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

int stringToPosInt(std::string const& str)
{
	int result = 0;
	std::istringstream iss(str);
	if (!(iss >> result))
		return -1;
	return result;
}

bool checkIp(std::string ip)
{
	std::vector<std::string> ipParts = split(ip, '.');
	if (ipParts.size() != 4)
		return false;
	for (std::size_t i = 0; i < ipParts.size(); i++)
	{
		if (!isPosNumber(ipParts[i]))
			return false;
		int ipPart = stringToPosInt(ipParts[i]);
		if (ipPart < 0 || ipPart > 255)
			return false;
	}
	return true;
}

bool checkEnable(std::string enable)
{
	if (enable == "on")
		return true;
	else if (enable == "off")
		return false;
	return false;
}

bool isBrace(std::string line)
{
	if (line.length() == 1 && (line.at(0) == '{' || line.at(0) == '}'))
		return true;
	return false;
}