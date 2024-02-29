
#ifndef CONFIG_FILE_PARSER_HPP
# define CONFIG_FILE_PARSER_HPP

class Location;
class VirtualServer;

# include "../includes/headers.hpp"

class ConfigFileParser
{
	private:
		// Attributes
		std::string						_buffer;
		std::string						_filename;
		std::vector<VirtualServer *>	_VServers;
		std::map<std::string, int>		_server_tokens;
		std::map<std::string, int>		_location_tokens;

		// Useless Constructors & Assignment
		ConfigFileParser();
		ConfigFileParser(const ConfigFileParser &);
		ConfigFileParser &operator=(const ConfigFileParser&);

		// Exceptions
		class invalid_syntax : public std::exception
		{
			public:
				const char *what() const throw ()
				{
					return ("ConfigFileParser: Syntax Error!");
				}
		};

	public:
		// Constructor & Destructor
		ConfigFileParser(const char *);
		~ConfigFileParser();
	private:
		// Global Helpers
		void	buffering_input_file(); // ! don't store comments
		int		skip_blank(const char *ptr, int start) const;
		int		skip_spaces(const char *ptr, int start) const;
		void	display_extracted_infos() const;
		void	check_extracted_infos() const;

	private:
		// ********************* Syntax Checking *********************

		// Syntax checker's Main Method
		void	syntax_checker() const;

		// Helpers
		int		count_and_skip_words(const char *ptr, int *index_ptr) const;

		// Server block syntax checkers
		int		server_block_syntax_checker(int start) const;
		int		server_tokens_syntax_checker(int start) const;
		int 	check_server_token(const char *ptr, int index) const;
		int		check_server_token_value(const char *ptr, int offset1, int offset2) const;

		// Location block syntax checkers
		int		location_syntax_checker(int start) const;
		int 	location_tokens_syntax_checker(int start) const;
		int		check_location_token_value(const char *ptr, int offset1, int offset2) const;
		int 	check_location_token(const char *ptr, int index) const;

	private:
		// ********************* Extracting config infos *********************

		// Helpers
		VirtualServer *new_virtual_server();

		// Extracting config infos Main Method
		void	extracting_config_infos();

		// Extracting server config infos
		int		extract_server_infos(int index);
		int		extract_server_token_values(VirtualServer *vs, int index);
		int		extract_server_token_value(VirtualServer *vs, const char *ptr, int index);

		// Extracting location config infos
		int		extract_location_infos(VirtualServer *vs, int index);
		int		extract_location_token_values(VirtualServer *vs, Location *loc, int index);
		int		extract_location_token_value(VirtualServer *vs, Location *loc, const char *ptr, int index);

	public:
		// Getters
		const std::vector<VirtualServer *> &get_parsing_result() const;

	public:
		// ConfigFileParser Main Method
		void	config_file_parsing();
};

# include "VirtualServer.hpp"

#endif
