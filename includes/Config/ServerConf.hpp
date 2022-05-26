/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:14:39 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/26 20:48:41 by eassouli         ###   ########.fr       */
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

typedef	std::map<std::string,void(*)(std::vector<std::string>&)> parseFunction_t;

// when get one info check if exist or if default possible like cgi folder or autoindex

class ServerConf {
	s_server						m_main;
	std::map<std::string, s_server>	m_subs;

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
	static void	findServer( std::ifstream &ifs );

	// Only server
	static void	parseListen( std::vector<std::string> &tokens );
	// static void	parseServerName( std::vector<std::string> &tokens );

	// Only location
	// static void	parseMethod( std::vector<std::string> &tokens );
	// static void	parseCgi( std::vector<std::string> &tokens );

	// Both server and location
	// static void	parseErrorPage( std::vector<std::string> &tokens );
	// static void	parseClientMaxBodySize( std::vector<std::string> &tokens );
	// static void	parseRedirect( std::vector<std::string> &tokens );
	// static void	parseRoot( std::vector<std::string> &tokens );
	// static void	parseAutoindex( std::vector<std::string> &tokens );
	// static void	parseIndex( std::vector<std::string> &tokens );
	// static void	parseUploadPass( std::vector<std::string> &tokens );

	// To remove
	const char	*getIp() const {
		return m_main.listen.first.c_str();
	}
	int	getPort() const {
		return m_main.listen.second;
	}
	//

	class ConfFail : public std::exception {
		// int	m_flag;
		// error messages
	public:
		// ConfFail( int flag ) : m_flag(flag) {}
		virtual const char	*what() const throw() {
			return "Conf failed";
		}
	};
};

#endif
