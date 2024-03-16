


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <cctype>

class MiniBlock{
	public:
		typedef bool (MiniBlock::*parseFunctions)(std::string);
		std::map<std::string, parseFunctions> fields;
		// #######   start of block (server or location) fields  ########
		std::string listen;
		std::string serverName;
		std::string root;
		std::vector<std::string> index;
		std::vector<std::string> allowMethods;
		std::string locationName;
		std::vector<MiniBlock> locations;
		// #######   end of block (server or location) fields  ########
		std::string getvalue(std::string line)
		{
			std::size_t found = line.find(" ");
			if (found != std::string::npos)
				return line.substr(found + 1);
			return "";
		}

		bool checkListen(std::string listen)
		{
			listen = getvalue(listen);
			if (listen.empty())
				return false;
			this->listen = listen;
			return true;
		}

		bool checkServerName(std::string serverName)
		{
			serverName = getvalue(serverName);
			if (serverName.empty())
				return false;
			this->serverName = serverName;
			return true;
		}
		bool checkRoot(std::string root)
		{
			root = getvalue(root);
			if (root.empty())
				return false;
			this->root = root;
			return true;
		}
		bool checkLocationName(std::string locationName)
		{
			locationName = getvalue(locationName);
			if (locationName.empty())
				return false;
			this->locationName = locationName;
			return true;
		}
		bool checkIndex(std::string index)
		{
			index = getvalue(index);
			if (index.empty())
				return false;
			std::istringstream iss(index);
			std::string temp;
			while (iss >> temp)
				this->index.push_back(temp);
			return true;
		}
		bool checkAllowMethods(std::string allowMethods)
		{
			allowMethods = getvalue(allowMethods);
			if (allowMethods.empty())
				return false;
			std::istringstream iss(allowMethods);
			std::string temp;
			while (iss >> temp)
				this->allowMethods.push_back(temp);
			return true;
		}
		std::string removeExtraSpaces(std::string const& exp)
		{
			std::string newStr;
			bool space = true;
			for(std::size_t i = 0; i < exp.length(); i++)
			{
				if (isspace(exp[i]) && !space)
				{
						newStr.push_back(' ');
						space =  true;
				}
				else if (!isspace(exp[i]))
				{
					space = false;
					newStr.push_back(exp[i]);
				}
			}
			if (newStr[newStr.length() - 1] == ' ')
				newStr.pop_back();
			return newStr;
		}

		std::vector<std::string> split(std::string line)
		{
			std::vector<std::string> result;
			line = removeExtraSpaces(line);
			std::istringstream iss(line);
			std::string temp;
			while (iss >> temp)
			{
				result.push_back(temp);
			}
			return result;
		}
	public:
		MiniBlock()
		{
			fields["listen"] = &MiniBlock::checkListen;
			fields["server_name"] = &MiniBlock::checkServerName;
			fields["root"] = &MiniBlock::checkRoot;
			fields["index"] = &MiniBlock::checkIndex;
			fields["allow_methods"] = &MiniBlock::checkAllowMethods;
			fields["location"] = &MiniBlock::checkLocationName;
		}
		~MiniBlock(){}
		bool parseline(std::string line)
		{
			std::cout << "line: " << line << std::endl;
			std::vector<std::string> result = split(line);
			std::string field = result[0];
			if (fields.find(field) == fields.end())
				return false;
			return (this->*fields[field])(line);
		}
};

// << operator overloading for MiniBlock
std::ostream& operator<<(std::ostream& os, const MiniBlock& mb)
{
	os << "listen: " << mb.listen << std::endl;
	if (mb.serverName.empty())
		os << "location_name: " << mb.locationName << std::endl;
	else
		os << "server_name: " << mb.serverName << std::endl;
	os << "root: " << mb.root << std::endl;
	os << "index: ";
	for (std::size_t i = 0; i < mb.index.size(); i++)
		os << mb.index[i] << " ";
	os << std::endl;
	os << "allow_methods: ";
	for (std::size_t i = 0; i < mb.allowMethods.size(); i++)
		os << mb.allowMethods[i] << " ";
	os << std::endl;
	for (std::size_t i = 0; i < mb.locations.size(); i++)
	{
		os << "########## location: ##########"<< std::endl;
		os << mb.locations[i] << std::endl;
	}
	return os;
}

std::string removeExtraSpaces(std::string const& exp)
{
	std::string newStr;
	bool space = true;
	for(std::size_t i = 0; i < exp.length(); i++)
	{
		if (isspace(exp[i]) && !space)
		{
				newStr.push_back(' ');
				space =  true;
		}
		else if (!isspace(exp[i]))
		{
			space = false;
			newStr.push_back(exp[i]);
		}
	}
	if (newStr[newStr.length() - 1] == ' ')
		newStr.pop_back();
	return newStr;
}

class parse{
	public:
		std::vector<MiniBlock> servers;
		MiniBlock server;
		MiniBlock location;
		std::ifstream configFile;
		std::stack<char> balance;
	public:
		parse(std::string filename)
		{
			configFile.open(filename.c_str());
			if (!configFile)
			{
				std::cerr << "Failed to open config file.\n";
				throw std::runtime_error("Failed to open config file.\n");
			}
		}
		~parse(){configFile.close();}
		void parseBlock(std::ifstream& configFile, std::string context)
		{
			std::string line;
			while (std::getline(configFile, line))
			{
				if (line.empty() || line.find("#") == 0)
					continue;
				line = removeExtraSpaces(line);
				if (line == "{")
				{
					balance.push('{');
					continue;
				}
				else if (line == "}")
				{
					if (balance.empty()){
						std::cerr << "Error: extra closing brace" << std::endl;
						return;
					}
					if (context == "server")
					{
						std::cout << "server: " << server << std::endl;
						servers.push_back(server);
						server = MiniBlock();
						balance.pop();
						return;
					}
					else if (context == "location")
					{
						server.locations.push_back(location);
						location = MiniBlock();
						balance.pop();
						return;
					}
				}
				else if (line == "server")
				{
					parseBlock(configFile, "server");
				}
				else if (line.find("location") != std::string::npos)
				{
					location.parseline(line);
					parseBlock(configFile, "location");
				}
				else
				{
					if (context == "server")
					{
						server.parseline(line);
					}
					else if (context == "location")
					{
						location.parseline(line);
					}
				}
			}
		}
		
};

void print(std::istringstream &is)
{
	std::string line;
	while (is >> line)
	{
		std::cout << line << std::endl;
	}
}



std::vector<std::string> split(std::string line)
{
	std::vector<std::string> result;
	std::istringstream iss(line);
	std::string temp;
	while (iss >> temp)
	{
		result.push_back(temp);
	}
	return result;
}

void printVector(std::vector<std::string> vec)
{
	for (std::size_t i = 0; i < vec.size(); i++)
		std::cout << "[" << vec[i] << "]" << std::endl;
}

bool checkBalance(std::string line)
{
	// line will be like "{kljkjjkhaf}"
	std::stack<char> balance;
	for (std::size_t i = 0; i < line.length(); i++)
	{
		if (line[i] == '{')
			balance.push('{');
		else if (line[i] == '}')
		{
			if (balance.empty())
				return false;
			balance.pop();
		}
	}
	return balance.empty();
}

int main()
{
	// std::string line = "      	  listen	   	     8080      	";
	// line = removeExtraSpaces(line);
	// std::istringstream iss(line);
	// std::string temp;
	// std::getline(iss, temp, ' ');
	// std::cout << temp << std::endl;
	// if (temp == "listen")
	// {
	// 	print(iss);
	// }
	// std::cout << "[" << removeExtraSpaces(line) << "]" << std::endl;
	// std::vector<std::string> result = split(removeExtraSpaces(line));
	// if (*(--result.end()) == ";")
	// 	result.pop_back();
	// else
	// 	std::cerr << "Error: missing semicolon" << std::endl;
	// printVector(result);
	// #############################  checkBalance  #############################
	parse p("config.conf");
	p.parseBlock(p.configFile, "server");
	std::cout << p.servers.size() << std::endl;
	for (std::size_t i = 0; i < p.servers.size(); i++)
	{
		std::cout << ">>>>>>>>>>>  Server " << i << " <<<<<<<<<<<" << std::endl;
		std::cout << p.servers[i] << std::endl;
	}
	return 0;
}
