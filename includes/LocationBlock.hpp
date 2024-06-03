


#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>

class LocationBlock
{
	private:
		typedef bool (LocationBlock::*parseFunctions)(std::vector<std::string>);

		std::map<std::string, parseFunctions> Fields;
	// #######   start of LocationBlock   ########

		std::string 						locationName;
		std::string 						root;
		std::vector<std::string>			indexes;
		bool 								autoIndex;
		std::vector<std::string> 			allowedMethods;
		std::map<std::string, std::string>	cgiPaths;
		std::string							uploadPath;
		bool 								uploadEnable;
		bool 								cgiEnable;
		std::string							returnPath;

	// #######   end of LocationBlock   ########
		bool	checkLocation(std::vector<std::string> location);
		bool	checkRoot(std::vector<std::string> root);
		bool	checkIndex(std::vector<std::string> index);
		bool	checkAutoIndex(std::vector<std::string> autoIndex);
		bool	checkAllowMethods(std::vector<std::string> allowMethods);
		bool	checkCgiPath(std::vector<std::string> cgiPath);
		bool	checkUploadPath(std::vector<std::string> uploadPath);
		bool	checkUploadEnable(std::vector<std::string> uploadEnable);
		bool	checkCgiEnable(std::vector<std::string> cgiEnable);
		bool	checkReturn(std::vector<std::string> returnPath);

	public:
		LocationBlock();
		~LocationBlock();

	//********** getters***********
		std::string const&							getLocationName() const;
		std::string const&							getRoot() const;
		std::vector<std::string> const&				getIndex() const;
		bool const&									getAutoIndex() const;
		std::vector<std::string> const&				getAllowMethods() const;
		std::map<std::string, std::string> const&	getCgiPaths() const;
		std::string const& 							getUploadPath() const;
		bool const& 								getUploadEnable() const;
		bool const& 								getCgiEnable() const;
		std::string const&							getReturnPath() const;

	// end of getters
		void initFieldsMap();
		void initFields();
		void parseLocationLine(std::vector<std::string> line);
		void checkLocationBlock(std::string const& root);
};

// << operator overload
std::ostream& operator<<(std::ostream& outstream, LocationBlock const& locationBlock);
typedef	std::map<std::string, LocationBlock>::const_iterator	mapIterType;

#endif

