


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>

// int main() {
//     std::ifstream file("config.conf");
//     std::string line;
//     std::stack<std::string> braces;
//     std::stack<std::string> contexts;

//     while (std::getline(file, line)) {
//         std::istringstream iss(line);
//         std::string word;
//         while (iss >> word) {
//             if (word == "{")
// 			{
//                 if (!contexts.empty()) {
//                     braces.push("{");
//                 }
// 				if (contexts.size() != braces.size())
// 				{
// 					std::cout << "Unbalanced braces! #1" << std::endl;
// 					return 1;
// 				}
//             }
// 			else if (word == "}")
// 			{
//                 if (braces.empty())
// 				{
//                     std::cout << "Unbalanced braces! #2" << std::endl;
//                     return 1;
//                 }
//                 braces.pop();
//                 if (!contexts.empty())
// 				{
//                     std::cout << "End of context: " << contexts.top() << std::endl;
//                     contexts.pop();
//                 }
//             }
// 			else
// 			{
// 				if (word == "server" && !contexts.empty())
// 				{
// 					std::cout << "server context already open" << std::endl;
// 					return 1;
// 				}
// 				else if (contexts.size() != braces.size())
// 				{
// 					std::cout << "Unbalanced braces! #3" << std::endl;
// 					return 1;
// 				}
// 				else if (word == "location" && (contexts.empty() || contexts.top() != "server"))
// 				{
// 					std::cout << "location context not in server context" << std::endl;
// 					return 1;
// 				}
// 				contexts.push(word);
// 				std::cout << "Start of context: " << word << std::endl;
// 			}
// 		}
// 	}

//     if (!braces.empty() || !contexts.empty()) {
//         std::cout << "Unbalanced braces! #4" << std::endl;
//         return 1;
//     }

//     std::cout << "Braces are balanced.\n";
//     return 0;
// }

void checkBraces(std::string const& configFile)
{
	std::ifstream file(configFile);
    std::string line;
    std::stack<std::string> braces;
    std::stack<std::string> contexts;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
			if (word == "{" && contexts.empty() == false)
					braces.push("{");
			else if (word == "}" && braces.empty())
					throw std::runtime_error("Config Error: check closing braces");
			else if (word == "}" && braces.empty() == false)
			{
				braces.pop();
				if (contexts.empty() == false)
				{
					std::cout << "End of context: " << contexts.top() << std::endl;
					contexts.pop();
				}
			}
			else if (word == "server" && contexts.empty() == false)
				throw std::runtime_error("Config Error: check server context");
			else if (word == "location" && contexts.empty())
				throw std::runtime_error("Config Error: location is outside server context");
			else if (word == "location" && contexts.top() != "server")
				throw std::runtime_error("Config Error: location is inside another location context");
			else
				contexts.push(word);
		}
	}

    if (!braces.empty() || !contexts.empty())
		throw std::runtime_error("Config Error: check braces #4");
}

int main() {
    
	try
	{
		checkBraces("config.conf");
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
    std::cout << "Braces are balanced.\n";
    return 0;
}