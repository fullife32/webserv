/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:14:39 by eassouli          #+#    #+#             */
/*   Updated: 2022/06/02 19:32:46 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
#include <fcntl.h>
#include <fstream>
#include "ConfStructs.hpp"
#include "utils.hpp"

# define CONFIG_BUFFER_SIZE 256
# define MAX_PORT 65535
# define LOCALHOST "127.0.0.1"
# define IP_MASK "255.255.255.255"
# define DEFAULT_PORT "8000"
# define DEFAULT_BODY 1000000
# define DEFAULT_CONFIG_PATH "test_config/simple.conf" // TODO actualise path for main

enum e_config_error {
	EMPTY_FILE,
	NO_FILE,
	NOT_FILE,
	ANOTHER_ERROR,
	OUTSIDE_SERVER,
	NOT_VALID_KEY,
	SERVER_MANDATORY,
	TOO_MANY_ARGUMENTS,
	NOT_ENOUGH_ARGUMENTS,
	TOO_MANY_DIRECTIVE,
	MISSING_END_LINE,
	INVALID_IP,
	INVALID_PORT,
	NO_CLOSING_BRACKET,
	WRONG_ERROR_HTTP_CODE,
	WRONG_BODY_SIZE_FORMAT,
	WRONG_REDIR_HTTP_CODE,
	AUTOINDEX_FORMAT,
	WRONG_METHOD,
	WRONG_PATH_FORMAT,
	WRONG_EXT_FORMAT
};

typedef	std::map<std::string,void(*)(std::vector<std::string>&,s_base&)> parseFunction_t;

class ServerConf {
	std::pair<std::vector<std::string>, s_server>	m_main; // TODO: if server name here send this or search map or this by default
	std::map<std::vector<std::string>, s_server>	m_subs;

public:
	ServerConf( std::vector<std::string> server_name, s_server serverConf );
	ServerConf( ServerConf const &other );
	~ServerConf();

private:
	ServerConf();
	ServerConf &operator=( ServerConf const &other );

public:
	static void showConf(std::vector<ServerConf> &confs); // DEBUG

	static void parseLocation( std::ifstream &ifs, struct s_location &location, parseFunction_t &locationFnct );
	static void	initLocationConf( struct s_location &config );

	static bool	insertInSub(s_server &newServer, std::vector<ServerConf> &confs);
	static void	replaceConfig(s_server &server, const s_location &location); // TODO really usefull ???
	static void	mandatoryCheck( struct s_server &config);
	static bool	isEnding( std::string &lastToken );
	static void	parseServer( std::ifstream	&ifs, struct s_server &block, parseFunction_t &serverFnct, parseFunction_t &locationFnct );
	static void	initServerConf( struct s_server &config );
	static bool	findServer( std::ifstream &ifs );

	static void	setFunctionsCall( parseFunction_t &serverFnct, parseFunction_t &locationFnct );
	static void	openFile( std::string filePath, std::ifstream &ifs );
	static int	startParse( const std::string &filePath, std::vector<ServerConf> &confs );

	// Server only
	static void	parseListen( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseServerName( std::vector<std::string> &tokens, struct s_base &block );

	// Location only
	static void	parseMethod( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseCgi( std::vector<std::string> &tokens, struct s_base &block );

	// Common functions
	static void	parseErrorPage( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseClientMaxBodySize( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseRedirect( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseRoot( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseAutoindex( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseIndex( std::vector<std::string> &tokens, struct s_base &block );
	static void	parseUploadPass( std::vector<std::string> &tokens, struct s_base &block );

	// Getter functions
	const char	*getIp() const;
	size_t		getPort() const;
	const s_base	&getLocationByName( std::string serverName, std::string locationName, bool &yes ) const;
	const s_server	&getServerByName( std::string name ) const;
	bool			isMethodAllowed( std::string server_name, std::string location, int method ) const;
	size_t  		isRedirecting(std::string server_name, std::string location, std::string &url) const;
	std::string  	getLocationPath(std::string server_name, std::string location) const;
	std::string		getErrorPage(std::string server_name, std::string location, size_t errorNumber) const;



	class ConfFail : public std::exception {
		int					m_flag;
		std::string			m_word;

	public:
		ConfFail( int flag, std::string word ) : m_flag(flag), m_word(word) {}
		virtual ~ConfFail() throw() {}
		virtual const char	*what() const throw() {
			char const *error_msg[] = {
				"The configuration file is empty",
				"No configuration file found",
				"Impossible to open the configuration file",
				"Impossible to read configuration file",
				"Something found outside a server block",
				"An invalid directive has been found",
				"A mandatory directive is missing to setup a server",
				"Too many arguments",
				"Not enough arguments",
				"Too many declaration of this directive",
				"Missing a ; at the end of the line",
				"Invalid host",
				"Invalid port",
				"No closing bracket found at the end of",
				"Invalid error http code number (4xx - 5xx)",
				"Invalid client max body size number",
				"Invalid redirection http code number (3xx)",
				"Invalid autoindex format (on | off)",
				"Invalid method (GET | POST | DELETE)",
				"Invalid path format (/path)",
				"Invalid extension format (.extension)"
			};
			static std::string	ret;
			if (m_word != "")
				ret = std::string(error_msg[m_flag]) + ": " + m_word;
			else
				ret = std::string(error_msg[m_flag]);
			return ret.c_str();
		}
	};
};

#endif
