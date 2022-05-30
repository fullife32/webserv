/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:14:39 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/30 22:56:52 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <fcntl.h>
#include <fstream>
#include "ConfStructs.hpp"
#include "utils.hpp"

# define BUFFER_SIZE 256
# define MAX_PORT 65535
# define LOCALHOST "127.0.0.1"
# define IP_MASK "255.255.255.255"
# define DEFAULT_PORT "8000"

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
	WRONG_REDIR_HTTP_CODE
};

typedef	std::map<std::string,void(*)(std::vector<std::string>&,s_server&)> parseFunction_t;

// when get one info check if exist or if default possible like cgi folder or autoindex

class ServerConf {
	s_server										m_main;
	std::map<std::vector<std::string>, s_server>	m_subs;

public:
	ServerConf( s_server serverConf );
	ServerConf( ServerConf const &other );
	~ServerConf();

private:
	ServerConf();
	ServerConf &operator=( ServerConf const &other );

public:
	static int	startParse( const std::string &filePath, std::vector<ServerConf> &confs );
	static void	openFile( std::string filePath, std::ifstream &ifs );
	static void	setFunctionsCall( parseFunction_t &serverFnct, parseFunction_t &locationFnct );
	static void	parseServer( std::ifstream	&ifs, struct s_server &block, parseFunction_t &serverFnct, parseFunction_t &locationFnct );
	// static void parseLocation( std::ifstream &ifs, struct s_location &location, parseFunction_t &locationFnct );
	static bool	findServer( std::ifstream &ifs );

	// Only server
	static void	parseListen( std::vector<std::string> &tokens, struct s_server &block );
	static void	parseServerName( std::vector<std::string> &tokens, struct s_server &block );

	// Only location
	// static void	parseMethod( std::vector<std::string> &tokens, struct s_server &block );
	// static void	parseCgi( std::vector<std::string> &tokens, struct s_server &block );

	// Both server and location
	static void	parseErrorPage( std::vector<std::string> &tokens, struct s_server &block );
	static void	parseClientMaxBodySize( std::vector<std::string> &tokens, struct s_server &block );
	static void	parseRedirect( std::vector<std::string> &tokens, struct s_server &block );
	// static void	parseRoot( std::vector<std::string> &tokens, struct s_server &block );
	// static void	parseAutoindex( std::vector<std::string> &tokens, struct s_server &block );
	// static void	parseIndex( std::vector<std::string> &tokens, struct s_server &block );
	// static void	parseUploadPass( std::vector<std::string> &tokens, struct s_server &block );

	// To remove
	const char	*getIp() const {
		return m_main.listen.first.c_str();
	}
	int	getPort() const {
		return m_main.listen.second;
	}
	//

	class ConfFail : public std::exception {
		int					m_flag;
		const char			*m_word;

	public:
		ConfFail( int flag, const char *word ) : m_flag(flag), m_word(word) {}
		virtual const char	*what() const throw() {
			char const *error_msg[] = {
				"The configuration file is empty",
				"No configuration file found",
				"Impossible to open the configuration file",
				"Something else occured in conf", // make it better
				"Something found outside a server block",
				"An invalid directive has been found",
				"Not enough directive to setup a server in the configuration file",
				"Too many arguments",
				"Not enough arguments",
				"Too many declaration of this directive",
				"Missing a ; at the end of the line",
				"Invalid host",
				"Invalid port",
				"No closing bracket found at the end of",
				"Invalid error http code number (4xx-5xx)",
				"Invalid client max body size number",
				"Invalid redirection http code number (3xx)"
			};
			static std::string	ret;
			if (m_word != NULL)
				ret = std::string(error_msg[m_flag]) + ": " + std::string(m_word);
			else
				ret = std::string(error_msg[m_flag]);
			return ret.c_str();
		}
	};
};

#endif
