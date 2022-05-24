/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eassouli <eassouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 11:14:39 by eassouli          #+#    #+#             */
/*   Updated: 2022/05/24 12:47:02 by eassouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP

#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <fcntl.h>
#include <fstream>
#include "ConfStructs.hpp"

// #define AUTH_LOGIN 0
// #define AUTH_PASS 1

// when get one info check if exist or if default possible like cgi folder or autoindex

class ServerConf {
	std::string	_ipAddr;
	std::string	_listen;
	s_server						m_main;
	std::map<std::string, s_server>	m_subs;

public:
	ServerConf(std::string ip, std::string port);
	~ServerConf();

private:
	ServerConf();
	ServerConf( ServerConf const &other );
	ServerConf &operator=( ServerConf const &other );

public:
	static int	checkFile( std::string filePath );

	// To remove
	const char *getIp() const {
		return _ipAddr.c_str();
	}
	int	getPort() const {
		return atoi(_listen.c_str());
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
