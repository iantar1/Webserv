

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct LocationBlock {
    std::string path;
    std::map<std::string, std::string> directives;
};

struct ServerBlock {
    std::vector<LocationBlock> locations;
    std::map<std::string, std::string> directives;
};

struct Config {
    std::vector<ServerBlock> servers;
};

void parseBlock(std::ifstream& configFile, ServerBlock& serverBlock, LocationBlock& locationBlock);

int main() {
    std::ifstream configFile("nginx.conf");
    if (!configFile) {
        std::cerr << "Failed to open config file.\n";
        return 1;
    }

    Config config;
    ServerBlock serverBlock;
    LocationBlock locationBlock;
    parseBlock(configFile, serverBlock, locationBlock);

    // Now you can access server and location blocks from the 'config' variable
    // and process them further as needed

    return 0;
}

void parseBlock(std::ifstream& configFile, ServerBlock& serverBlock, LocationBlock& locationBlock) {
    std::string line;
    while (std::getline(configFile, line)) {
        // Skip empty lines and comments
        if (line.empty() || line.find("#") == 0)
            continue;

        // Parse server block
        if (line.find("server") != std::string::npos) {
            if (!serverBlock.directives.empty()) {
                serverBlock.locations.push_back(locationBlock);
                locationBlock = LocationBlock();
            }
            parseBlock(configFile, serverBlock, locationBlock);
        }
        // Parse location block
        else if (line.find("location") != std::string::npos) {
            parseBlock(configFile, serverBlock, locationBlock);
        }
        // Parse directives
        else if (line.find("}") != std::string::npos) {
            return; // End of block
        }
        else {
            // Parse directives and add them to appropriate block
        }
    }
}







void parseBlock(std::ifstream& configFile, Config& config, ServerBlock& serverBlock, LocationBlock& locationBlock, std::string context) {
    std::string line;
    while (std::getline(configFile, line)) {
        // Skip empty lines and comments
        if (line.empty() || line.find("#") == 0)
            continue;

        // Parse server block
        if (line.find("server") != std::string::npos) {
            if (context == "server") {
                config.servers.push_back(serverBlock);
                serverBlock = ServerBlock();
            } else if (context == "location") {
                serverBlock.locations.push_back(locationBlock);
                locationBlock = LocationBlock();
            }
            parseBlock(configFile, config, serverBlock, locationBlock, "server");
        }
        // Parse location block
        else if (line.find("location") != std::string::npos) {
            if (context == "server") {
                serverBlock.locations.push_back(locationBlock);
                locationBlock = LocationBlock();
            }
            parseBlock(configFile, config, serverBlock, locationBlock, "location");
        }
        // Parse directives
        else if (line.find("}") != std::string::npos) {
            if (context == "server") {
                config.servers.push_back(serverBlock);
            } else if (context == "location") {
                serverBlock.locations.push_back(locationBlock);
            }
            return; // End of block
        }
        else {
            // Parse directives and add them to appropriate block
            // Here you need to add logic to parse individual directives and add them to serverBlock or locationBlock
        }
    }
}